/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef MOZ_UNITS_H_
#define MOZ_UNITS_H_

#include "mozilla/gfx/Point.h"
#include "mozilla/gfx/Rect.h"
#include "mozilla/gfx/ScaleFactor.h"
#include "nsDeviceContext.h"

namespace mozilla {

struct CSSPixel;
struct LayerPixel;
struct ScreenPixel;

typedef gfx::PointTyped<CSSPixel> CSSPoint;
typedef gfx::IntPointTyped<CSSPixel> CSSIntPoint;
typedef gfx::SizeTyped<CSSPixel> CSSSize;
typedef gfx::IntSizeTyped<CSSPixel> CSSIntSize;
typedef gfx::RectTyped<CSSPixel> CSSRect;
typedef gfx::IntRectTyped<CSSPixel> CSSIntRect;

typedef gfx::PointTyped<LayerPixel> LayerPoint;
typedef gfx::IntPointTyped<LayerPixel> LayerIntPoint;
typedef gfx::SizeTyped<LayerPixel> LayerSize;
typedef gfx::IntSizeTyped<LayerPixel> LayerIntSize;
typedef gfx::RectTyped<LayerPixel> LayerRect;
typedef gfx::IntRectTyped<LayerPixel> LayerIntRect;

typedef gfx::PointTyped<ScreenPixel> ScreenPoint;
typedef gfx::IntPointTyped<ScreenPixel> ScreenIntPoint;
typedef gfx::SizeTyped<ScreenPixel> ScreenSize;
typedef gfx::IntSizeTyped<ScreenPixel> ScreenIntSize;
typedef gfx::RectTyped<ScreenPixel> ScreenRect;
typedef gfx::IntRectTyped<ScreenPixel> ScreenIntRect;

typedef gfx::ScaleFactor<CSSPixel, LayerPixel> CSSToLayerScale;
typedef gfx::ScaleFactor<LayerPixel, CSSPixel> LayerToCSSScale;
typedef gfx::ScaleFactor<CSSPixel, ScreenPixel> CSSToScreenScale;
typedef gfx::ScaleFactor<ScreenPixel, CSSPixel> ScreenToCSSScale;
typedef gfx::ScaleFactor<LayerPixel, ScreenPixel> LayerToScreenScale;
typedef gfx::ScaleFactor<ScreenPixel, LayerPixel> ScreenToLayerScale;

/*
 * The pixels that content authors use to specify sizes in.
 */
struct CSSPixel {

  // Conversions from app units

  static CSSPoint FromAppUnits(const nsPoint& aPoint) {
    return CSSPoint(NSAppUnitsToFloatPixels(aPoint.x, float(nsDeviceContext::AppUnitsPerCSSPixel())),
                    NSAppUnitsToFloatPixels(aPoint.y, float(nsDeviceContext::AppUnitsPerCSSPixel())));
  }

  static CSSRect FromAppUnits(const nsRect& aRect) {
    return CSSRect(NSAppUnitsToFloatPixels(aRect.x, float(nsDeviceContext::AppUnitsPerCSSPixel())),
                   NSAppUnitsToFloatPixels(aRect.y, float(nsDeviceContext::AppUnitsPerCSSPixel())),
                   NSAppUnitsToFloatPixels(aRect.width, float(nsDeviceContext::AppUnitsPerCSSPixel())),
                   NSAppUnitsToFloatPixels(aRect.height, float(nsDeviceContext::AppUnitsPerCSSPixel())));
  }

  static CSSIntPoint FromAppUnitsRounded(const nsPoint& aPoint) {
    return CSSIntPoint(NSAppUnitsToIntPixels(aPoint.x, float(nsDeviceContext::AppUnitsPerCSSPixel())),
                       NSAppUnitsToIntPixels(aPoint.y, float(nsDeviceContext::AppUnitsPerCSSPixel())));
  }

  static CSSIntRect FromAppUnitsRounded(const nsRect& aRect) {
    return CSSIntRect(NSAppUnitsToIntPixels(aRect.x, float(nsDeviceContext::AppUnitsPerCSSPixel())),
                      NSAppUnitsToIntPixels(aRect.y, float(nsDeviceContext::AppUnitsPerCSSPixel())),
                      NSAppUnitsToIntPixels(aRect.width, float(nsDeviceContext::AppUnitsPerCSSPixel())),
                      NSAppUnitsToIntPixels(aRect.height, float(nsDeviceContext::AppUnitsPerCSSPixel())));
  }

  // Conversions to app units

  static nsPoint ToAppUnits(const CSSPoint& aPoint) {
    return nsPoint(NSFloatPixelsToAppUnits(aPoint.x, float(nsDeviceContext::AppUnitsPerCSSPixel())),
                   NSFloatPixelsToAppUnits(aPoint.y, float(nsDeviceContext::AppUnitsPerCSSPixel())));
  }

  static nsPoint ToAppUnits(const CSSIntPoint& aPoint) {
    return nsPoint(NSIntPixelsToAppUnits(aPoint.x, nsDeviceContext::AppUnitsPerCSSPixel()),
                   NSIntPixelsToAppUnits(aPoint.y, nsDeviceContext::AppUnitsPerCSSPixel()));
  }

  static nsRect ToAppUnits(const CSSRect& aRect) {
    return nsRect(NSFloatPixelsToAppUnits(aRect.x, float(nsDeviceContext::AppUnitsPerCSSPixel())),
                  NSFloatPixelsToAppUnits(aRect.y, float(nsDeviceContext::AppUnitsPerCSSPixel())),
                  NSFloatPixelsToAppUnits(aRect.width, float(nsDeviceContext::AppUnitsPerCSSPixel())),
                  NSFloatPixelsToAppUnits(aRect.height, float(nsDeviceContext::AppUnitsPerCSSPixel())));
  }
};

/*
 * The pixels that layout rasterizes and delivers to the graphics code.
 * These are generally referred to as "device pixels" in layout code. Layer
 * pixels are affected by:
 * 1) the "display resolution" (see nsIPresShell::SetResolution)
 * 2) the "full zoom" (see nsPresContext::SetFullZoom)
 * 3) the "widget scale" (nsIWidget::GetDefaultScale)
 */
struct LayerPixel {
};

/*
 * The pixels that are displayed on the screen.
 * On non-OMTC platforms this should be equivalent to LayerPixel units.
 * On OMTC platforms these may diverge from LayerPixel units temporarily,
 * while an asynchronous zoom is happening, but should eventually converge
 * back to LayerPixel units. Some variables (such as those representing
 * chrome UI element sizes) that are not subject to content zoom should
 * generally be represented in ScreenPixel units.
 */
struct ScreenPixel {

  // Conversions to CSS units

  static CSSIntRect ToCSSIntRectRoundIn(const ScreenIntRect& aRect, float aResolutionX, float aResolutionY) {
    CSSIntRect ret(aRect.x, aRect.y, aRect.width, aRect.height);
    ret.ScaleInverseRoundIn(aResolutionX, aResolutionY);
    return ret;
  }
};

// Operators to apply ScaleFactors directly to Points and Rects

template<class src, class dst>
gfx::PointTyped<dst> operator*(const gfx::PointTyped<src>& aPoint, const gfx::ScaleFactor<src, dst>& aScale) {
  return gfx::PointTyped<dst>(aPoint.x * aScale.scale,
                              aPoint.y * aScale.scale);
}

template<class src, class dst>
gfx::PointTyped<dst> operator/(const gfx::PointTyped<src>& aPoint, const gfx::ScaleFactor<dst, src>& aScale) {
  return gfx::PointTyped<dst>(aPoint.x / aScale.scale,
                              aPoint.y / aScale.scale);
}

template<class src, class dst>
gfx::RectTyped<dst> operator*(const gfx::RectTyped<src>& aRect, const gfx::ScaleFactor<src, dst>& aScale) {
  return gfx::RectTyped<dst>(aRect.x * aScale.scale,
                             aRect.y * aScale.scale,
                             aRect.width * aScale.scale,
                             aRect.height * aScale.scale);
}

template<class src, class dst>
gfx::RectTyped<dst> operator/(const gfx::RectTyped<src>& aRect, const gfx::ScaleFactor<dst, src>& aScale) {
  return gfx::RectTyped<dst>(aRect.x / aScale.scale,
                             aRect.y / aScale.scale,
                             aRect.width / aScale.scale,
                             aRect.height / aScale.scale);
}

};

#endif
