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

#include "optiondata.h"

#include "exception.h"

#include <QDebug>
#include <QFont>
#include <QFontDatabase>

OptionData *OptionData::optionData = nullptr;

/* Default values for well known networks */

OptionData::OptionData()
{

}

OptionData::~OptionData()
{

}

opts::OnlineFormat OptionData::getOnlineFormat() const
{
  switch(onlineNetwork)
  {
    case opts::ONLINE_CUSTOM:
    case opts::ONLINE_CUSTOM_STATUS:
    case opts::ONLINE_NONE:
      return onlineFormat;

    case opts::ONLINE_VATSIM:
      return opts::ONLINE_FORMAT_VATSIM;

    case opts::ONLINE_IVAO:
      return opts::ONLINE_FORMAT_IVAO;

    case opts::ONLINE_PILOTEDGE:
      return opts::ONLINE_FORMAT_VATSIM;
  }
  return opts::ONLINE_FORMAT_VATSIM;
}

QString OptionData::getOnlineStatusUrl() const
{
  switch(onlineNetwork)
  {
    case opts::ONLINE_CUSTOM:
    case opts::ONLINE_NONE:
      return QString();

    case opts::ONLINE_VATSIM:
      return onlineVatsimStatusUrl;

    case opts::ONLINE_IVAO:
      return onlineIvaoStatusUrl;

    case opts::ONLINE_PILOTEDGE:
      return onlinePilotEdgeStatusUrl;

    case opts::ONLINE_CUSTOM_STATUS:
      return onlineStatusUrl;
  }
  return QString();
}

QString OptionData::getOnlineWhazzupUrl() const
{
  switch(onlineNetwork)
  {
    case opts::ONLINE_NONE:
      return QString();

    case opts::ONLINE_CUSTOM:
      return onlineWhazzupUrl;

    case opts::ONLINE_VATSIM:
    case opts::ONLINE_IVAO:
    case opts::ONLINE_PILOTEDGE:
    case opts::ONLINE_CUSTOM_STATUS:
      return QString();
  }
  return QString();
}

QFont OptionData::getMapFont() const
{
  QFont font;
  if(!mapFont.isEmpty())
    font.fromString(mapFont);
  else if(!guiFont.isEmpty())
    font.fromString(guiFont);
  else
    font = QFontDatabase::systemFont(QFontDatabase::GeneralFont);
  return font;
}

const OptionData& OptionData::instance()
{
  OptionData& optData = instanceInternal();

  if(!optData.valid)
  {
    qCritical() << "OptionData not initialized yet";
    throw atools::Exception("OptionData not initialized yet");
  }

  return optData;
}

OptionData& OptionData::instanceInternal()
{
  if(optionData == nullptr)
  {
    qDebug() << "Creating new OptionData";
    optionData = new OptionData();
  }

  return *optionData;
}
