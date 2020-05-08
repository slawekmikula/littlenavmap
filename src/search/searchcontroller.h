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

#include "common/mapflags.h"

#include "common/tabindexes.h"

#include <QObject>

#ifndef LITTLENAVMAP_SEARCHCONTROLLER_H
#define LITTLENAVMAP_SEARCHCONTROLLER_H

class QMainWindow;
class AbstractSearch;
class AirportSearch;
class NavSearch;
class ProcedureSearch;
class UserdataSearch;
class LogdataSearch;
class OnlineClientSearch;
class OnlineCenterSearch;
class OnlineServerSearch;
class ColumnList;
class QTableView;
class QTabWidget;
class QTreeWidget;
class MapQuery;

namespace atools {
namespace gui {
class TabWidgetHandler;

}
namespace sql {
class SqlRecord;
}

namespace geo {
class Pos;
}
}

namespace map {
struct MapSearchResult;

}

/*
 * Manages all search tabs.
 */
class SearchController :
  public QObject
{
  Q_OBJECT

public:
  SearchController(QMainWindow *parent, QTabWidget *tabWidgetSearchParam);
  virtual ~SearchController();

  /* Create the airport search tab */
  void createAirportSearch(QTableView *tableView);

  /* Create the navaid search tab */
  void createNavSearch(QTableView *tableView);

  void createProcedureSearch(QTreeWidget *treeWidget);

  void createUserdataSearch(QTableView *tableView);
  void createLogdataSearch(QTableView *tableView);

  void createOnlineClientSearch(QTableView *tableView);
  void createOnlineCenterSearch(QTableView *tableView);
  void createOnlineServerSearch(QTableView *tableView);

  AirportSearch *getAirportSearch() const
  {
    return airportSearch;
  }

  NavSearch *getNavSearch() const
  {
    return navSearch;
  }

  ProcedureSearch *getProcedureSearch() const
  {
    return procedureSearch;
  }

  UserdataSearch *getUserdataSearch() const
  {
    return userdataSearch;
  }

  LogdataSearch *getLogdataSearch() const
  {
    return logdataSearch;
  }

  OnlineClientSearch *getOnlineClientSearch() const
  {
    return onlineClientSearch;
  }

  OnlineCenterSearch *getOnlineCenterSearch() const
  {
    return onlineCenterSearch;
  }

  OnlineServerSearch *getOnlineServerSearch() const
  {
    return onlineServerSearch;
  }

  /* Disconnect and reconnect all queries if a new database is loaded or changed */
  void preDatabaseLoad();
  void postDatabaseLoad();

  /* Save table view and search parameters to settings file */
  void saveState();
  void restoreState();

  /* Reset search and show the given type in the search result. Search widgets are populated with the
   * given parameters. Types can be airport, VOR, NDB or waypoint */
  void showInSearch(map::MapObjectTypes type, const atools::sql::SqlRecord& record, bool select);

  /* Get all selected airports or navaids from the active search tab */
  void getSelectedMapObjects(map::MapSearchResult& result) const;

  /* Options have changed. Update table font, empty airport handling etc. */
  void optionsChanged();

  /* GUI style has changed */
  void styleChanged();

  /* Refresh after import or changes */
  void refreshUserdata();

  void refreshLogdata();

  /* Clear selection in all search windows  */
  void clearSelection();

  /* True if any of the search windows has a selection */
  bool hasSelection();

  void setCurrentSearchTabId(si::TabSearchId tabId);

  si::TabSearchId getCurrentSearchTabId();

  void resetWindowLayout();

private:
  void tabChanged(int index);
  void postCreateSearch(AbstractSearch *search);
  void helpPressed();
  void helpPressedProcedure();
  void helpPressedUserdata();
  void helpPressedOnlineClient();
  void helpPressedOnlineCenter();
  void helpPressedLogdata();

  MapQuery *mapQuery;

  AirportSearch *airportSearch = nullptr;
  NavSearch *navSearch = nullptr;
  ProcedureSearch *procedureSearch = nullptr;
  UserdataSearch *userdataSearch = nullptr;
  LogdataSearch *logdataSearch = nullptr;
  OnlineClientSearch *onlineClientSearch = nullptr;
  OnlineCenterSearch *onlineCenterSearch = nullptr;
  OnlineServerSearch *onlineServerSearch = nullptr;

  QMainWindow *mainWindow = nullptr;
  QTabWidget *tabWidgetSearch = nullptr;
  QList<AbstractSearch *> allSearchTabs;

  atools::gui::TabWidgetHandler *tabHandlerSearch = nullptr;
};

#endif // LITTLENAVMAP_SEARCHCONTROLLER_H
