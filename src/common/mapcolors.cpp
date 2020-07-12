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

#include "common/mapcolors.h"

#include "query/mapquery.h"
#include "options/optiondata.h"
#include "settings/settings.h"
#include "atools.h"

#include <QPen>
#include <QString>
#include <QApplication>
#include <QPalette>
#include <QSettings>
#include <QPainter>
#include <navapp.h>

namespace mapcolors {

/* Colors that are updated from confguration file */

QPen taxiwayLinePen(QColor(200, 200, 0), 1.5, Qt::DashLine, Qt::FlatCap);
QColor taxiwayNameColor(Qt::black);

QColor taxiwayNameBackgroundColor(255, 255, 120);

QColor airportDetailBackColor(255, 255, 255);
QColor airportEmptyColor(130, 130, 130);
QColor toweredAirportColor(15, 70, 130);
QColor unToweredAirportColor(126, 58, 91);
QColor vorSymbolColor(Qt::darkBlue);
QColor ndbSymbolColor(Qt::darkRed);
QColor markerSymbolColor(Qt::darkMagenta);
QColor ilsSymbolColor(Qt::darkGreen);

QPen ilsCenterPen(ilsSymbolColor, 1.5, Qt::DashLine);

QColor ilsFillColor("#40008000");

QColor ilsTextColor(0, 30, 0);

QColor waypointSymbolColor(200, 0, 200);

QColor airwayVictorColor("#969696"); // 1.
QColor airwayJetColor("#000080"); // 1.
QColor airwayBothColor("#646464"); // 1.
QColor airwayTrackColor("#101010"); // 1.5
QColor airwayTextColor(80, 80, 80);

QColor rangeRingColor(Qt::red);
QColor rangeRingTextColor(Qt::black);
QColor distanceColor(Qt::black);

QColor weatherWindGustColor("#ff8040");
QColor weatherWindColor(Qt::black);
QColor weatherBackgoundColor(Qt::white);

QColor weatherLifrColor(QColor("#d000d0"));
QColor weatherIfrColor(QColor("#d00000"));
QColor weatherMvfrColor(QColor("#0000d0"));
QColor weatherVfrColor(QColor("#00b000"));

QPen minimumAltitudeGridPen(QColor("#a0a0a0"), 1.);
QColor minimumAltitudeNumberColor(QColor("#70000000"));

QColor compassRoseColor(Qt::darkRed);
QColor compassRoseTextColor(Qt::black);

/* Elevation profile colors and pens */
QColor profileSkyColor(QColor(204, 204, 255));
QColor profileLandColor(QColor(0, 128, 0));
QColor profileLabelColor(QColor(0, 0, 0));

QColor profileVasiAboveColor(QColor("#70ffffff"));
QColor profileVasiBelowColor(QColor("#70ff0000"));

QColor profileAltRestrictionFill(QColor(255, 255, 90));
QColor profileAltRestrictionOutline(Qt::black);

QPen profileVasiCenterPen(Qt::darkGray, 1.5, Qt::DashLine);
QPen profileLandOutlinePen(Qt::black, 1, Qt::SolidLine);
QPen profileWaypointLinePen(Qt::gray, 1, Qt::SolidLine, Qt::FlatCap);
QPen profileElevationScalePen(Qt::gray, 1, Qt::SolidLine, Qt::FlatCap);
QPen profileSafeAltLinePen(Qt::red, 4, Qt::SolidLine, Qt::FlatCap);
QPen profileSafeAltLegLinePen(QColor(255, 100, 0), 3, Qt::SolidLine, Qt::FlatCap);

/* Objects highlighted because of selection in search */
QColor highlightBackColor(Qt::black);
QColor highlightColor(Qt::yellow);
QColor highlightColorFast(Qt::darkYellow);

/* Objects highlighted because of selection in route table */
QColor routeHighlightBackColor(Qt::black);
QColor routeHighlightColor(Qt::green);
QColor routeHighlightColorFast(Qt::darkGreen);

/* Objects highlighted because of selection in route profile */
QColor profileHighlightBackColor(Qt::black);
QColor profileHighlightColor(Qt::cyan);
QColor profileHighlightColorFast(Qt::darkCyan);

/* Map print colors */
QColor mapPrintRowColor(250, 250, 250);
QColor mapPrintRowColorAlt(240, 240, 240);
QColor mapPrintHeaderColor(220, 220, 220);

QPen searchCenterBackPen(QColor(0, 0, 0), 6, Qt::SolidLine, Qt::FlatCap);
QPen searchCenterFillPen(QColor(255, 255, 0), 2, Qt::SolidLine, Qt::FlatCap);
QPen touchMarkBackPen(QColor(0, 0, 0), 4, Qt::SolidLine, Qt::FlatCap);
QPen touchMarkFillPen(QColor(255, 255, 255), 2, Qt::SolidLine, Qt::FlatCap);
QColor touchRegionFillColor("#40888888");

/* Alternating colors */
static QColor rowBgColor;
static QColor rowAltBgColor;

/* Slightly darker background for sort column */
static QColor rowSortBgColor;
static QColor rowSortAltBgColor;

void styleChanged()
{
  rowBgColor = QApplication::palette().color(QPalette::Active, QPalette::Base);
  rowAltBgColor = QApplication::palette().color(QPalette::Active, QPalette::AlternateBase);
  rowSortBgColor = rowBgColor.darker(106);
  rowSortAltBgColor = rowAltBgColor.darker(106);
}

void init()
{
  styleChanged();
}

const QColor& colorForAirport(const map::MapAirport& ap)
{
  if(ap.emptyDraw())
    return airportEmptyColor;
  else if(ap.tower())
    return toweredAirportColor;
  else
    return unToweredAirportColor;
}

const QColor& alternatingRowColor(int row, bool isSort)
{
  if((row % 2) == 0)
    return isSort ? rowSortBgColor : rowBgColor;
  else
    return isSort ? rowSortAltBgColor : rowAltBgColor;
}

const QColor& colorOutlineForParkingType(const QString& type)
{
  if(type == "RMCB" || type == "RMC" || type.startsWith("G") || type.startsWith("RGA") || type.startsWith("DGA") ||
     type.startsWith("RC") || type.startsWith("FUEL") || type == ("H") || type == ("T"))
    return parkingOutlineColor;
  else
    return parkingUnknownOutlineColor;
}

const QColor& colorForParkingType(const QString& type)
{
  static const QColor rampMilCargo(190, 0, 0);
  static const QColor rampMilCombat(Qt::red);
  static const QColor rampMil(190, 0, 0);
  static const QColor gate(100, 100, 255);
  static const QColor rampGa(0, 200, 0);
  static const QColor tiedown(0, 150, 0);
  static const QColor hangar(Qt::darkYellow);
  static const QColor rampCargo(Qt::darkGreen);
  static const QColor fuel(Qt::yellow);
  static const QColor unknown("#808080");

  if(type == QLatin1Literal("RM"))
    return rampMil;
  else if(type == QLatin1Literal("RMC"))
    return rampMilCargo;
  else if(type == QLatin1Literal("RMCB"))
    return rampMilCombat;
  else if(type.startsWith(QLatin1Literal("G")))
    return gate;
  else if(type.startsWith(QLatin1Literal("RGA")) || type.startsWith(QLatin1Literal("DGA")))
    return rampGa;
  else if(type.startsWith(QLatin1Literal("RC")))
    return rampCargo;
  else if(type.startsWith(QLatin1Literal("FUEL")))
    return fuel;
  else if(type == (QLatin1Literal("H")))
    return hangar;
  else if(type == (QLatin1Literal("T")))
    return tiedown;
  else
    return unknown;
}

const QColor& colorTextForParkingType(const QString& type)
{
  if(type == "RMCB")
    return mapcolors::brightParkingTextColor;
  else if(type == "RMC")
    return mapcolors::brightParkingTextColor;
  else if(type.startsWith("G"))
    return mapcolors::brightParkingTextColor;
  else if(type.startsWith("RGA") || type.startsWith("DGA"))
    return mapcolors::brightParkingTextColor;
  else if(type.startsWith("RC"))
    return mapcolors::brightParkingTextColor;
  else if(type.startsWith("FUEL"))
    return mapcolors::darkParkingTextColor;
  else if(type == ("H"))
    return brightParkingTextColor;
  else if(type == ("T"))
    return brightParkingTextColor;
  else
    return brightParkingTextColor;
}

const QIcon& iconForStartType(const QString& type)
{
  static const QIcon runway(":/littlenavmap/resources/icons/startrunway.svg");
  static const QIcon helipad(":/littlenavmap/resources/icons/starthelipad.svg");
  static const QIcon water(":/littlenavmap/resources/icons/startwater.svg");

  static const QIcon empty;
  if(type == "R")
    return runway;
  else if(type == "H")
    return helipad;
  else if(type == "R")
    return water;
  else
    return empty;
}

const QIcon& iconForParkingType(const QString& type)
{
  static const QIcon cargo(":/littlenavmap/resources/icons/parkingrampcargo.svg");
  static const QIcon ga(":/littlenavmap/resources/icons/parkingrampga.svg");
  static const QIcon mil(":/littlenavmap/resources/icons/parkingrampmil.svg");
  static const QIcon gate(":/littlenavmap/resources/icons/parkinggate.svg");
  static const QIcon fuel(":/littlenavmap/resources/icons/parkingfuel.svg");
  static const QIcon hangar(":/littlenavmap/resources/icons/parkinghangar.svg");
  static const QIcon tiedown(":/littlenavmap/resources/icons/parkingtiedown.svg");
  static const QIcon unknown(":/littlenavmap/resources/icons/parkingunknown.svg");

  if(type.startsWith("RM"))
    return mil;
  else if(type.startsWith("G"))
    return gate;
  else if(type.startsWith("RGA") || type.startsWith("DGA"))
    return ga;
  else if(type.startsWith("RC"))
    return cargo;
  else if(type.startsWith("FUEL"))
    return fuel;
  else if(type == ("H"))
    return hangar;
  else if(type == ("T"))
    return tiedown;
  else
    return unknown;
}

const QColor& colorForSurface(const QString& surface)
{
  static const QColor concrete("#888888");
  static const QColor grass("#00a000");
  static const QColor water("#808585ff");
  static const QColor asphalt("#707070");
  static const QColor cement("#d0d0d0");
  static const QColor clay("#DEB887");
  static const QColor snow("#dbdbdb");
  static const QColor ice("#d0d0ff");
  static const QColor dirt("#CD853F");
  static const QColor coral("#FFE4C4");
  static const QColor gravel("#c0c0c0");
  static const QColor oilTreated("#2F4F4F");
  static const QColor steelMats("#a0f0ff");
  static const QColor bituminous("#808080");
  static const QColor brick("#A0522D");
  static const QColor macadam("#c8c8c8");
  static const QColor planks("#8B4513");
  static const QColor sand("#F4A460");
  static const QColor shale("#F5DEB3");
  static const QColor tarmac("#909090");
  static const QColor unknown("#ffffff");
  static const QColor transparent("#ffffff");

  if(surface == "A")
    return asphalt;
  else if(surface == "G")
    return grass;
  else if(surface == "D")
    return dirt;
  else if(surface == "C")
    return concrete;
  else if(surface == "GR")
    return gravel;
  else if(surface == "W")
    return water;
  else if(surface == "CE")
    return cement;
  else if(surface == "CL")
    return clay;
  else if(surface == "SN")
    return snow;
  else if(surface == "I")
    return ice;
  else if(surface == "CR")
    return coral;
  else if(surface == "OT")
    return oilTreated;
  else if(surface == "SM")
    return steelMats;
  else if(surface == "B")
    return bituminous;
  else if(surface == "BR")
    return brick;
  else if(surface == "M")
    return macadam;
  else if(surface == "PL")
    return planks;
  else if(surface == "S")
    return sand;
  else if(surface == "SH")
    return shale;
  else if(surface == "T")
    return tarmac;
  else if(surface == "TR")
    return transparent;

  // else if(surface == "NONE" || surface == "UNKNOWN" || surface == "INVALID") || TR
  return unknown;
}

const QPen aircraftTrailPen(float size)
{
  opts::DisplayTrailType type = OptionData::instance().getDisplayTrailType();

  switch(type)
  {
    case opts::DASHED:
      return QPen(OptionData::instance().getTrailColor(), size, Qt::DashLine, Qt::FlatCap, Qt::BevelJoin);

    case opts::DOTTED:
      return QPen(OptionData::instance().getTrailColor(), size, Qt::DotLine, Qt::FlatCap, Qt::BevelJoin);

    case opts::SOLID:
      return QPen(OptionData::instance().getTrailColor(), size, Qt::SolidLine, Qt::FlatCap, Qt::BevelJoin);
  }
  return QPen();
}

static QHash<map::MapAirspaceTypes, QColor> airspaceFillColors(
  {
    {map::AIRSPACE_NONE, QColor("#00000000")},
    {map::CENTER, QColor("#30808080")},
    {map::CLASS_A, QColor("#308d0200")},
    {map::CLASS_B, QColor("#30902ece")},
    {map::CLASS_C, QColor("#308594ec")},
    {map::CLASS_D, QColor("#306c5bce")},
    {map::CLASS_E, QColor("#30cc5060")},
    {map::CLASS_F, QColor("#307d8000")},
    {map::CLASS_G, QColor("#30cc8040")},
    {map::FIR, QColor("#30606080")},
    {map::UIR, QColor("#30404080")},
    {map::TOWER, QColor("#300000f0")},
    {map::CLEARANCE, QColor("#3060808a")},
    {map::GROUND, QColor("#30000000")},
    {map::DEPARTURE, QColor("#3060808a")},
    {map::APPROACH, QColor("#3060808a")},
    {map::MOA, QColor("#304485b7")},
    {map::RESTRICTED, QColor("#30fd8c00")},
    {map::PROHIBITED, QColor("#30f00909")},
    {map::WARNING, QColor("#30fd8c00")},
    {map::CAUTION, QColor("#50fd8c00")},
    {map::ALERT, QColor("#30fd8c00")},
    {map::DANGER, QColor("#30dd103d")},
    {map::NATIONAL_PARK, QColor("#30509090")},
    {map::MODEC, QColor("#30509090")},
    {map::RADAR, QColor("#30509090")},
    {map::TRAINING, QColor("#30509090")},
    {map::GLIDERPROHIBITED, QColor("#30fd8c00")},
    {map::WAVEWINDOW, QColor("#304485b7")},
    {map::ONLINE_OBSERVER, QColor("#3000a000")}
  }
  );

static QHash<map::MapAirspaceTypes, QPen> airspacePens(
  {
    {map::AIRSPACE_NONE, QPen(QColor("#00000000"))},
    {map::CENTER, QPen(QColor("#808080"), 1.5)},
    {map::CLASS_A, QPen(QColor("#8d0200"), 2)},
    {map::CLASS_B, QPen(QColor("#902ece"), 2)},
    {map::CLASS_C, QPen(QColor("#8594ec"), 2)},
    {map::CLASS_D, QPen(QColor("#6c5bce"), 2)},
    {map::CLASS_E, QPen(QColor("#cc5060"), 2)},
    {map::CLASS_F, QPen(QColor("#7d8000"), 2)},
    {map::CLASS_G, QPen(QColor("#cc8040"), 2)},
    {map::FIR, QPen(QColor("#606080"), 1.5)},
    {map::UIR, QPen(QColor("#404080"), 1.5)},
    {map::TOWER, QPen(QColor("#6000a0"), 2)},
    {map::CLEARANCE, QPen(QColor("#60808a"), 2)},
    {map::GROUND, QPen(QColor("#000000"), 2)},
    {map::DEPARTURE, QPen(QColor("#60808a"), 2)},
    {map::APPROACH, QPen(QColor("#60808a"), 2)},
    {map::MOA, QPen(QColor("#4485b7"), 2)},
    {map::RESTRICTED, QPen(QColor("#fd8c00"), 2)},
    {map::PROHIBITED, QPen(QColor("#f00909"), 3)},
    {map::WARNING, QPen(QColor("#fd8c00"), 2)},
    {map::CAUTION, QPen(QColor("#ff6c00"), 2)},
    {map::ALERT, QPen(QColor("#fd8c00"), 2)},
    {map::DANGER, QPen(QColor("#dd103d"), 2)},
    {map::NATIONAL_PARK, QPen(QColor("#509090"), 2)},
    {map::MODEC, QPen(QColor("#509090"), 2)},
    {map::RADAR, QPen(QColor("#509090"), 2)},
    {map::TRAINING, QPen(QColor("#509090"), 2)},
    {map::GLIDERPROHIBITED, QPen(QColor("#fd8c00"), 2)},
    {map::WAVEWINDOW, QPen(QColor("#4485b7"), 2)},
    {map::ONLINE_OBSERVER, QPen(QColor("#a000a000"), 1.5)}
  }
  );

static QHash<QString, map::MapAirspaceTypes> airspaceConfigNames(
  {
    {"Center", map::CENTER},
    {"ClassA", map::CLASS_A},
    {"ClassB", map::CLASS_B},
    {"ClassC", map::CLASS_C},
    {"ClassD", map::CLASS_D},
    {"ClassE", map::CLASS_E},
    {"ClassF", map::CLASS_F},
    {"ClassG", map::CLASS_G},
    {"FIR", map::FIR},
    {"UIR", map::UIR},
    {"Tower", map::TOWER},
    {"Clearance", map::CLEARANCE},
    {"Ground", map::GROUND},
    {"Departure", map::DEPARTURE},
    {"Approach", map::APPROACH},
    {"Moa", map::MOA},
    {"Restricted", map::RESTRICTED},
    {"Prohibited", map::PROHIBITED},
    {"Warning", map::WARNING},
    {"Caution", map::WARNING},
    {"Alert", map::ALERT},
    {"Danger", map::DANGER},
    {"NationalPark", map::NATIONAL_PARK},
    {"Modec", map::MODEC},
    {"Radar", map::RADAR},
    {"Training", map::TRAINING},
    {"GliderProhibited", map::GLIDERPROHIBITED},
    {"WaveWindow", map::WAVEWINDOW},
    {"Observer", map::ONLINE_OBSERVER}
  }
  );

const QColor& colorForAirspaceFill(const map::MapAirspace& airspace)
{
  return airspaceFillColors[airspace.type];
}

const QPen& penForAirspace(const map::MapAirspace& airspace)
{
  return airspacePens[airspace.type];
}

const QColor& colorForAirwayTrack(const map::MapAirway& airway)
{
  static QColor EMPTY_COLOR;

  switch(airway.type)
  {
    case map::NO_AIRWAY:
      break;

    case map::TRACK_NAT:
    case map::TRACK_PACOTS:
    case map::TRACK_AUSOTS:
      return airwayTrackColor;

    case map::AIRWAY_VICTOR:
      return airwayVictorColor;

    case map::AIRWAY_JET:
      return airwayJetColor;

    case map::AIRWAY_BOTH:
      return airwayBothColor;
  }
  return EMPTY_COLOR;
}

/* Read ARGB color if value exists in settings or update in settings with given value */
void syncColorArgb(QSettings& settings, const QString& key, QColor& color)
{
  if(settings.contains(key))
    color.setNamedColor(settings.value(key).toString());
  else
    settings.setValue(key, color.name(QColor::HexArgb));
}

/* Read color if value exists in settings or update in settings with given value */
void syncColor(QSettings& settings, const QString& key, QColor& color)
{
  if(settings.contains(key))
    color.setNamedColor(settings.value(key).toString());
  else
    settings.setValue(key, color.name());
}

/* Read color and pen width if value exists in settings or update in settings with values of given pen */
void syncPen(QSettings& settings, const QString& key, QPen& pen)
{
  static QHash<QString, Qt::PenStyle> penToStyle(
    {
      {"Solid", Qt::SolidLine},
      {"Dash", Qt::DashLine},
      {"Dot", Qt::DotLine},
      {"DashDot", Qt::DashDotLine},
      {"DashDotDot", Qt::DashDotDotLine},
    });
  static QHash<Qt::PenStyle, QString> styleToPen(
    {
      {Qt::SolidLine, "Solid"},
      {Qt::DashLine, "Dash"},
      {Qt::DotLine, "Dot"},
      {Qt::DashDotLine, "DashDot"},
      {Qt::DashDotDotLine, "DashDotDot"},
    });

  if(settings.contains(key))
  {
    QStringList list = settings.value(key).toStringList();
    if(list.size() >= 1)
    {
      pen.setColor(QColor(list.at(0)));

      if(list.size() >= 2)
        pen.setWidthF(list.at(1).toFloat());

      if(list.size() >= 3)
        pen.setStyle(penToStyle.value(list.at(2), Qt::SolidLine));
    }
  }
  else
    settings.setValue(key, QStringList({pen.color().name(),
                                        QString::number(pen.widthF()),
                                        styleToPen.value(pen.style(), "Solid")}));
}

void syncColors()
{
#ifndef DEBUG_DISABLE_SYNC_COLORS
  QString filename = atools::settings::Settings::instance().getConfigFilename("_mapstyle.ini");

  QSettings colorSettings(filename, QSettings::IniFormat);
  colorSettings.setValue("Options/Version", QApplication::applicationVersion());

  colorSettings.beginGroup("Airport");
  syncColor(colorSettings, "DiagramBackgroundColor", airportDetailBackColor);
  syncColor(colorSettings, "EmptyColor", airportEmptyColor);
  syncColor(colorSettings, "ToweredColor", toweredAirportColor);
  syncColor(colorSettings, "UnToweredColor", unToweredAirportColor);
  syncPen(colorSettings, "TaxiwayLinePen", taxiwayLinePen);
  syncColor(colorSettings, "TaxiwayNameColor", taxiwayNameColor);
  syncColor(colorSettings, "TaxiwayNameBackgroundColor", taxiwayNameBackgroundColor);

  colorSettings.endGroup();

  colorSettings.beginGroup("Navaid");
  syncColor(colorSettings, "VorColor", vorSymbolColor);
  syncColor(colorSettings, "NdbColor", ndbSymbolColor);
  syncColor(colorSettings, "MarkerColor", markerSymbolColor);
  syncColor(colorSettings, "IlsColor", ilsSymbolColor);
  syncColorArgb(colorSettings, "IlsFillColor", ilsFillColor);
  syncColor(colorSettings, "IlsTextColor", ilsTextColor);
  syncPen(colorSettings, "IlsCenterPen", ilsCenterPen);
  syncColor(colorSettings, "WaypointColor", waypointSymbolColor);
  colorSettings.endGroup();

  colorSettings.beginGroup("Airway");
  syncColor(colorSettings, "VictorColor", airwayVictorColor);
  syncColor(colorSettings, "JetColor", airwayJetColor);
  syncColor(colorSettings, "BothColor", airwayBothColor);
  syncColor(colorSettings, "TrackColor", airwayTrackColor);
  syncColor(colorSettings, "TextColor", airwayTextColor);
  colorSettings.endGroup();

  colorSettings.beginGroup("Marker");
  syncColor(colorSettings, "DistanceGreatCircleColor", distanceColor);
  syncColor(colorSettings, "RangeRingColor", rangeRingColor);
  syncColor(colorSettings, "RangeRingTextColor", rangeRingTextColor);
  syncColor(colorSettings, "CompassRoseColor", compassRoseColor);
  syncColor(colorSettings, "CompassRoseTextColor", compassRoseTextColor);
  syncPen(colorSettings, "SearchCenterBackPen", searchCenterBackPen);
  syncPen(colorSettings, "SearchCenterFillPen", searchCenterFillPen);
  syncPen(colorSettings, "TouchMarkBackPen", touchMarkBackPen);
  syncPen(colorSettings, "TouchMarkFillPen", touchMarkFillPen);
  syncColorArgb(colorSettings, "TouchRegionFillColor", touchRegionFillColor);
  colorSettings.endGroup();

  colorSettings.beginGroup("Highlight");
  syncColor(colorSettings, "HighlightBackColor", highlightBackColor);
  syncColor(colorSettings, "HighlightColor", highlightColor);
  syncColor(colorSettings, "HighlightColorFast", highlightColorFast);
  syncColor(colorSettings, "RouteHighlightBackColor", routeHighlightBackColor);
  syncColor(colorSettings, "RouteHighlightColor", routeHighlightColor);
  syncColor(colorSettings, "RouteHighlightColorFast", routeHighlightColorFast);
  syncColor(colorSettings, "ProfileHighlightBackColor", profileHighlightBackColor);
  syncColor(colorSettings, "ProfileHighlightColor", profileHighlightColor);
  syncColor(colorSettings, "ProfileHighlightColorFast", profileHighlightColorFast);
  colorSettings.endGroup();

  colorSettings.beginGroup("Print");
  syncColor(colorSettings, "MapPrintRowColor", mapPrintRowColor);
  syncColor(colorSettings, "MapPrintRowColorAlt", mapPrintRowColorAlt);
  syncColor(colorSettings, "MapPrintHeaderColor", mapPrintHeaderColor);
  colorSettings.endGroup();

  colorSettings.beginGroup("Weather");
  syncColor(colorSettings, "WeatherBackgoundColor", weatherBackgoundColor);
  syncColor(colorSettings, "WeatherWindColor", weatherWindColor);
  syncColor(colorSettings, "WeatherWindGustColor", weatherWindGustColor);
  syncColor(colorSettings, "WeatherLifrColor", weatherLifrColor);
  syncColor(colorSettings, "WeatherIfrColor", weatherIfrColor);
  syncColor(colorSettings, "WeatherMvfrColor", weatherMvfrColor);
  syncColor(colorSettings, "WeatherVfrColor", weatherVfrColor);
  colorSettings.endGroup();

  colorSettings.beginGroup("AltitudeGrid");
  syncPen(colorSettings, "MinimumAltitudeGridPen", minimumAltitudeGridPen);
  syncColorArgb(colorSettings, "MinimumAltitudeNumberColor", minimumAltitudeNumberColor);
  colorSettings.endGroup();

  colorSettings.beginGroup("Profile");
  syncColor(colorSettings, "SkyColor", profileSkyColor);
  syncColor(colorSettings, "LandColor", profileLandColor);
  syncColor(colorSettings, "LabelColor", profileLabelColor);
  syncColorArgb(colorSettings, "VasiAboveColor", profileVasiAboveColor);
  syncColorArgb(colorSettings, "VasiBelowColor", profileVasiBelowColor);
  syncColor(colorSettings, "AltRestrictionFill", profileAltRestrictionFill);
  syncColor(colorSettings, "AltRestrictionOutline", profileAltRestrictionOutline);
  syncPen(colorSettings, "LandOutlinePen", profileLandOutlinePen);
  syncPen(colorSettings, "WaypointLinePen", profileWaypointLinePen);
  syncPen(colorSettings, "ElevationScalePen", profileElevationScalePen);
  syncPen(colorSettings, "SafeAltLinePen", profileSafeAltLinePen);
  syncPen(colorSettings, "SafeAltLegLinePen", profileSafeAltLegLinePen);
  syncPen(colorSettings, "VasiCenterPen", profileVasiCenterPen);
  colorSettings.endGroup();

  // Sync airspace colors ============================================
  colorSettings.beginGroup("Airspace");
  for(const QString& name : airspaceConfigNames.keys())
  {
    map::MapAirspaceTypes type = airspaceConfigNames.value(name);
    syncPen(colorSettings, name + "Pen", airspacePens[type]);
    syncColorArgb(colorSettings, name + "FillColor", airspaceFillColors[type]);
  }
  colorSettings.endGroup();

  colorSettings.sync();
#endif
}

void adjustPenForCircleToLand(QPainter *painter)
{
  // Use different pattern and smaller line for circle-to-land approaches
  QPen pen = painter->pen();
  pen.setStyle(Qt::DotLine);
  pen.setCapStyle(Qt::FlatCap);
  // pen.setWidthF(pen.widthF() * 0.80);
  painter->setPen(pen);
}

void adjustPenForVectors(QPainter *painter)
{
  // Use different pattern and smaller line for vector legs
  QPen pen = painter->pen();
  pen.setStyle(Qt::DashLine);
  pen.setCapStyle(Qt::FlatCap);
  // pen.setWidthF(pen.widthF() * 0.80);
  painter->setPen(pen);
}

void adjustPenForManual(QPainter *painter)
{
  // Use different pattern and smaller line for vector legs
  QPen pen = painter->pen();
  // The pattern must be specified as an even number of positive entries
  // where the entries 1, 3, 5... are the dashes and 2, 4, 6... are the spaces.
  pen.setDashPattern({1., 3.});
  pen.setCapStyle(Qt::FlatCap);
  // pen.setWidthF(pen.widthF() * 0.80);
  painter->setPen(pen);
}

void adjustPenForAlternate(QPainter *painter)
{
  // Use different pattern and smaller line for vector legs
  QPen pen = painter->pen();
  pen.setStyle(Qt::DotLine);
  pen.setCapStyle(Qt::FlatCap);
  painter->setPen(pen);
  painter->setBackground(Qt::white);
  painter->setBackgroundMode(Qt::OpaqueMode);
}

void scaleFont(QPainter *painter, float scale, const QFont *defaultFont)
{
  QFont font = painter->font();
  const QFont *defFont = defaultFont == nullptr ? &font : defaultFont;
  if(font.pixelSize() == -1)
  {
    // Use point size if pixel is not available
    double size = scale * defFont->pointSizeF();
    if(atools::almostNotEqual(size, font.pointSizeF()))
    {
      font.setPointSizeF(size);
      painter->setFont(font);
    }
  }
  else
  {
    int size = atools::roundToInt(scale * defFont->pixelSize());
    if(size != defFont->pixelSize())
    {
      font.setPixelSize(size);
      painter->setFont(font);
    }
  }
}

void darkenPainterRect(QPainter& painter)
{
  // Dim the map by drawing a semi-transparent black rectangle
  if(NavApp::isCurrentGuiStyleNight())
  {
    int dim = OptionData::instance().getGuiStyleMapDimming();
    QColor col = QColor::fromRgb(0, 0, 0, 255 - (255 * dim / 100));
    painter.fillRect(QRect(0, 0, painter.device()->width(), painter.device()->height()), col);
  }
}

} // namespace mapcolors
