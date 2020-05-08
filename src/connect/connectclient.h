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

#ifndef LITTLENAVMAP_CONNECTCLIENT_H
#define LITTLENAVMAP_CONNECTCLIENT_H

#include "fs/sc/simconnectdata.h"
#include "util/timedcache.h"
#include "connectdialog.h"

#include <QAbstractSocket>
#include <QCache>
#include <QTimer>

class QTcpSocket;
class ConnectDialog;
class MainWindow;

namespace atools {
namespace fs {
namespace sc {
class DataReaderThread;
class SimConnectHandler;
class XpConnectHandler;
class FgConnectHandler;
class ConnectHandler;
class WeatherRequest;
struct MetarResult;

class SimConnectReply;
}
}
}

/*
 * Client for the Little Navconnect Simconnect agent/server. Receives data and passes it around by emitting a signal.
 * Does not use multithreading - runs completely in the event loop.
 */
class ConnectClient :
  public QObject
{
  Q_OBJECT

public:
  ConnectClient(MainWindow *parent);
  virtual ~ConnectClient();

  /* Opens the connect dialog and depending on result connects to the server/agent */
  void connectToServerDialog();

  /* Connects directly if the connect on startup option is set */
  void tryConnectOnStartup();

  /* true if connected to Little Navconnect or the simulator */
  bool isConnected() const;

  /* true if connection is using SimConnect for FSX/P3D */
  bool isSimConnect() const;

  /* Connected to Little Navconnect */
  bool isConnectedNetwork() const;

  /* Just saves and restores the state of the dialog */
  void saveState();
  void restoreState();

  /* Request weather. Return value will be empty and the request will be started in background.
   * Signal weatherUpdated is sent if request was finished. Than call this method again.
   * onlyStation: Do not return weather for interpolated or nearest only. Keeps an internal blacklist. */
  atools::fs::weather::MetarResult requestWeather(const QString& station, const atools::geo::Pos& pos,
                                                  bool onlyStation);

  bool isFetchAiShip() const;
  bool isFetchAiAircraft() const;

signals:
  /* Emitted when new data was received from the server (Little Navconnect), SimConnect or X-Plane.
   * can be aircraft position or weather update */
  void dataPacketReceived(atools::fs::sc::SimConnectData simConnectData);

  /* Emitted when a new SimConnect data was received that contains weather data */
  void weatherUpdated();

  /* Emitted when a connection was established */
  void connectedToSimulator();

  /* Emitted when disconnected manually or due to error */
  void disconnectedFromSimulator();

  /* Fetch boat or aircraft AI has been changed */
  void aiFetchOptionsChanged();

private:
  /* Try to reconnect every 5 seconds when network connection is lost */
  const int SOCKET_RECONNECT_SEC = 5;
  /* Try to reconnect every 5 seconds when the SimConnect or X-Plane connection is lost */
  const int DIRECT_RECONNECT_SEC = 5;
  const int FLUSH_QUEUE_MS = 50;

  /* Any metar fetched from the Simulator will time out in 15 seconds */
  const int WEATHER_TIMEOUT_FS_SECS = 15;
  const int NOT_AVAILABLE_TIMEOUT_FS_SECS = 300;

  void readFromSocket();
  void readFromSocketError(QAbstractSocket::SocketError error);
  void connectedToServerSocket();
  void closeSocket(bool allowRestart);
  void connectInternal();
  void writeReplyToSocket(atools::fs::sc::SimConnectReply& reply);
  void disconnectClicked();
  void postSimConnectData(atools::fs::sc::SimConnectData dataPacket);
  void postLogMessage(QString message, bool warning);
  void connectedToSimulatorDirect();
  void disconnectedFromSimulatorDirect();
  void autoConnectToggled(bool state);
  void requestWeather(const atools::fs::sc::WeatherRequest& weatherRequest);
  void flushQueuedRequests();
  atools::fs::sc::ConnectHandler *handlerByDialogSettings();
  QString simShortName() const;
  QString simName() const;

  /* Options in ConnectDialog have changed */
  void fetchOptionsChanged(cd::ConnectSimType type);
  void directUpdateRateChanged(cd::ConnectSimType type);

  bool silent = false, manualDisconnect = false;
  ConnectDialog *dialog = nullptr;

  /* Does automatic reconnect */
  atools::fs::sc::DataReaderThread *dataReader = nullptr;
  atools::fs::sc::SimConnectHandler *simConnectHandler = nullptr;
  atools::fs::sc::XpConnectHandler *xpConnectHandler = nullptr;
  atools::fs::sc::FgConnectHandler *fgConnectHandler = nullptr;

  /* Have to keep it since it is read multiple times */
  atools::fs::sc::SimConnectData *simConnectData = nullptr;

  QTcpSocket *socket = nullptr;
  /* Used to trigger reconnects on socket base connections */
  QTimer reconnectNetworkTimer, flushQueuedRequestsTimer;
  MainWindow *mainWindow;
  bool verbose = false;
  atools::util::TimedCache<QString, atools::fs::weather::MetarResult> metarIdentCache;

  /* Waiting for these replies for airport idents */
  QSet<QString> outstandingReplies;

  /* Requests in queue */
  QVector<atools::fs::sc::WeatherRequest> queuedRequests;
  QSet<QString> queuedRequestIdents;

  /* Cache holding all weather stations that do not allow a direct report but rather interpolated or nearest */
  atools::util::TimedCache<QString, QString> notAvailableStations;

  // have to remember state separately to avoid sending signals when autoconnect fails
  bool socketConnected = false;
};

#endif // LITTLENAVMAP_CONNECTCLIENT_H
