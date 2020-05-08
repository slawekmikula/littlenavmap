/*****************************************************************************
* Copyright 2015-2020 Alexander Barthel alex@littlenavmap.org
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/

#include "mappainter/mappainterils.h"

#include "mapgui/mapscale.h"
#include "mapgui/maplayer.h"
#include "query/mapquery.h"
#include "geo/calculations.h"
#include "common/mapcolors.h"
#include "mapgui/mapwidget.h"
#include "util/paintercontextsaver.h"

#include <QElapsedTimer>

#include <marble/GeoPainter.h>

using namespace Marble;
using namespace atools::geo;
using map::MapIls;

MapPainterIls::MapPainterIls(MapPaintWidget* mapWidget, MapScale *mapScale)
  : MapPainter(mapWidget, mapScale)
{
}

MapPainterIls::~MapPainterIls()
{
}

void MapPainterIls::render(PaintContext *context)
{
  if(!context->objectTypes.testFlag(map::ILS))
    return;

  if(context->mapLayer->isIls())
  {
    const GeoDataLatLonBox& curBox = context->viewport->viewLatLonAltBox();

    const QList<MapIls> *ilsList = mapQuery->getIls(curBox, context->mapLayer, context->lazyUpdate);
    if(ilsList != nullptr)
    {
      atools::util::PainterContextSaver saver(context->painter);
      Q_UNUSED(saver);

      for(const MapIls& ils : *ilsList)
      {
        int x, y;
        // Need to get the real ILS size on the screen for mercator projection - otherwise feather may vanish
        bool visible = wToS(ils.position, x, y, scale->getScreeenSizeForRect(ils.bounding));

        if(!visible)
          // Check bounding rect for visibility
          visible = ils.bounding.overlaps(context->viewportRect);

        if(visible)
        {
          if(context->objCount())
            return;

          drawIlsSymbol(context, ils);
        }
      }
    }
  }
}

void MapPainterIls::drawIlsSymbol(const PaintContext *context, const map::MapIls& ils)
{
  atools::util::PainterContextSaver saver(context->painter);

  context->painter->setBackgroundMode(Qt::TransparentMode);
  context->painter->setBrush(mapcolors::ilsFillColor);
  context->painter->setPen(QPen(mapcolors::ilsSymbolColor, 2, Qt::SolidLine, Qt::FlatCap));

  QSize size = scale->getScreeenSizeForRect(ils.bounding);
  bool visible;
  // Use visible dummy here since we need to call the method that also returns coordinates outside the screen
  QPoint pmid = wToS(ils.posmid, size, &visible);
  QPoint origin = wToS(ils.position, size, &visible);

  QPoint p1 = wToS(ils.pos1, size, &visible);
  QPoint p2 = wToS(ils.pos2, size, &visible);

  if(ils.slope > 0.1f)
  {
    context->painter->drawPolygon(QPolygonF({origin, p1, p2, origin}));
    context->painter->drawPolyline(QPolygonF({p1, pmid, p2}));
  }
  else
    context->painter->drawPolygon(QPolygonF({origin, p1, pmid, p2, origin}));

  context->painter->setPen(QPen(mapcolors::ilsCenterPen));
  context->painter->drawLine(origin, pmid);

  if(!context->drawFast)
  {
    // Draw ILS text -----------------------------------
    QString text;
    if(context->mapLayer->isIlsInfo())
      text = map::ilsText(ils);
    else if(context->mapLayer->isIlsIdent())
      text = ils.ident;

    if(!text.isEmpty())
    {
      context->szFont(context->textSizeNavaid);
      context->painter->setPen(QPen(mapcolors::ilsTextColor, 0.5f, Qt::SolidLine, Qt::FlatCap));
      context->painter->translate(origin);

      float width = ils.width < map::INVALID_COURSE_VALUE ? ils.width : map::DEFAULT_ILS_WIDTH;

      // Rotate to draw the text upwards so it is readable
      float rotate;
      if(ils.heading > 180)
        rotate = ils.heading + 90.f - width / 2.f;
      else
        rotate = atools::geo::opposedCourseDeg(ils.heading) + 90.f + width / 2.f;

      // get an approximation of the ILS length
      int featherLen = static_cast<int>(std::roundf(scale->getPixelForMeter(nmToMeter(FEATHER_LEN_NM), rotate)));

      if(featherLen > MIN_LENGHT_FOR_TEXT)
      {
        QFontMetrics metrics = context->painter->fontMetrics();
        int texth = metrics.descent();

        // Cut text to feather length
        text = metrics.elidedText(text, Qt::ElideRight, featherLen);
        int textw = metrics.width(text);

        int textpos;
        if(ils.heading > 180)
          textpos = (featherLen - textw) / 2;
        else
          textpos = -(featherLen + textw) / 2;

        if(context->flags2 & opts2::MAP_NAVAID_TEXT_BACKGROUND)
        {
          context->painter->setBackground(Qt::white);
          context->painter->setBackgroundMode(Qt::OpaqueMode);
        }

        context->painter->rotate(rotate);
        context->painter->drawText(textpos, -texth, text);
        context->painter->resetTransform();
      }
    }
  }
}
