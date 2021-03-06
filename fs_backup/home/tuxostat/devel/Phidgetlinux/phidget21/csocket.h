#ifndef __CSOCKET
#define __CSOCKET

#include "cphidget.h"
#include "cphidgetmanager.h"
#include "cphidgetdictionary.h"

#ifndef EXTERNALPROTO

extern CThread_mutex_t serverLock;
extern CThread_mutex_t zeroconfPhidgetsLock;
extern CPhidgetListHandle zeroconfPhidgets;
extern CPhidgetRemoteListHandle zeroconfServers;
extern CPhidgetListHandle zeroconfPhidgets;
extern CThread_mutex_t zeroconfServersLock;
extern CThread_mutex_t zeroconfPhidgetsLock;
extern CPhidgetListHandle activeRemotePhidgets;
extern CPhidgetManagerListHandle activeRemoteManagers;
extern CPhidgetDictionaryListHandle activeRemoteDictionaries;
extern CThread_mutex_t activeRemotePhidgetsLock;
extern CThread_mutex_t activeRemoteManagersLock;
extern CThread_mutex_t activeRemoteDictionariesLock;

void internal_async_network_error_handler(const char *error, void *phid);
int unregisterRemotePhidget(CPhidgetHandle phid);
int unregisterRemoteDictionary(CPhidgetDictionaryHandle dict);
int unregisterRemoteManager(CPhidgetManagerHandle phidm);
int CPhidgetSocketClient_clearStatusFlag(CPhidgetSocketClientHandle socket, int flag);
int CPhidgetSocketClient_setStatusFlag(CPhidgetSocketClientHandle socket, int flag);
int CPhidgetSocketClient_statusFlagIsSet(CPhidgetSocketClientHandle socket, int flag);
CThread_func_return_t DisconnectPhidgetThreadFunction(CThread_func_arg_t lpdwParam);
int getZeroconfHostPort(CPhidgetRemoteHandle networkInfo);
int CCONV CPhidgetRemote_areEqual(void *arg1, void *arg2);
void CCONV CPhidgetRemote_free(void *arg);
int CCONV CPhidgetRemote_create(CPhidgetRemoteHandle *arg);

PHIDGET21_API void CCONV CPhidgetSocketClient_free(void *arg);
PHIDGET21_API int CCONV CPhidgetSocketClient_areEqual(void *arg1, void *arg2);
PHIDGET21_API int CCONV CPhidgetSocketClient_create(CPhidgetSocketClientHandle *arg);

#endif

/** \addtogroup phidcommon
 * @{
 */
/**
 * Opens a Phidget remotely by ServerID. Note that this requires Bonjour (mDNS) to be running on both the host and the server.
 * @param phid A phidget handle.
 * @param serial Serial number. Specify -1 to open any.
 * @param serverID Server ID. Specify NULL to open any.
 * @param password Password. Can be NULL if the server is running unsecured.
 */
PHIDGET21_API int CCONV CPhidget_openRemote(CPhidgetHandle phid, int serial, const char *serverID, const char *password);
/**
 * Opens a Phidget remotely by address and port.
 * @param phid A phidget handle.
 * @param serial Serial number. Specify -1 to open any.
 * @param address Address. This can be a hostname or IP address.
 * @param port Port number. Default is 5001.
 * @param password Password. Can be NULL if the server is running unsecured.
 */
PHIDGET21_API int CCONV CPhidget_openRemoteIP(CPhidgetHandle phid, int serial, const char *address, int port, const char *password);
/** @} */

/** \addtogroup phidmanager
 * @{
 */
/**
 * Opens a Phidget manager remotely by ServerID. Note that this requires Bonjour (mDNS) to be running on both the host and the server.
 * @param phidm A phidget manager handle.
 * @param serverID Server ID. Specify NULL to open any.
 * @param password Password. Can be NULL if the server is running unsecured.
 */
PHIDGET21_API int CCONV CPhidgetManager_openRemote(CPhidgetManagerHandle phidm, const char *serverID, const char *password);
/**
 * Opens a Phidget manager remotely by address and port.
 * @param phidm A phidget manager handle.
 * @param address Address. This can be a hostname or IP address.
 * @param port Port number. Default is 5001.
 * @param password Password. Can be NULL if the server is running unsecured.
 */
PHIDGET21_API int CCONV CPhidgetManager_openRemoteIP(CPhidgetManagerHandle phidm, const char *address, int port, const char *password);
/** @} */

/** \addtogroup phiddict
 * @{
 */
/**
 * Opens a Phidget dictionary by ServerID. Note that this requires Bonjour (mDNS) to be running on both the host and the server.
 * @param dict A phidget dictionary handle.
 * @param serverID Server ID. Specify NULL to open any.
 * @param password Password. Can be NULL if the server is running unsecured.
 */
PHIDGET21_API int CCONV CPhidgetDictionary_openRemote(CPhidgetDictionaryHandle dict, const char *serverID, const char *password);
/**
 * Opens a Phidget dictionary by address and port.
 * @param dict A phidget dictionary handle.
 * @param address Address. This can be a hostname or IP address.
 * @param port Port number. Default is 5001.
 * @param password Password. Can be NULL if the server is running unsecured.
 */
PHIDGET21_API int CCONV CPhidgetDictionary_openRemoteIP(CPhidgetDictionaryHandle dict, const char *address, int port, const char *password);
/** @} */

#endif
