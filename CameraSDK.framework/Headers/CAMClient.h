//
//  CAMClient.h
//  CameraSDK
//
//  Created by Arvin on 2021/12/06.
//  Copyright © 2019 Askey. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CoreGraphics/CoreGraphics.h>
#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

typedef void(^kCAMConnectCameraBlock)(int errorCode,int sid, int avIndex);
typedef void(^kCAMReceiveVideoBlock)(UIImage *image,NSData *imageData);
typedef void(^kCAMConvertCompleteBlock)(BOOL isSuccess);
typedef void(^kCAMSolutionBlock)(BOOL isSuccess, int width, int height);

typedef void(^kCAMDownloadVideoProgressBlock)(int complete,int total, BOOL *stop);
typedef void(^kCAMDownloadVideoFinishBlock)(BOOL isSuccess);

typedef enum : NSUInteger {
    CAMEvent_Type_All = 0x0,
    CAMEvent_Type_Motion = 0x1,
    CAMEvent_Type_PIR = 0x4,
    CAMEvent_Type_Volume = 0x40,
} CAMEventType;

typedef struct
{
    unsigned int  total;        // Total bytes
    unsigned char index;        // package index, 0,1,2...;
    unsigned char endflag;        // end flag; endFlag = 1 means this package is the last one.
    unsigned short count;        // how much bytes in this package
    unsigned char result[0];    // The first memory address of the parameter in this package
}AMEGIA_SMsgAVIoctrlDataC;

/**
 Device information found in the LAN
 */
@interface CAMCameraInfo : NSObject


@property (strong, nonatomic) NSString *uid;
@property (strong, nonatomic) NSString *ip;
@property (strong, nonatomic) NSString *macAddress;
@property (strong, nonatomic) NSNumber *port;

+ (CAMCameraInfo *)initWithUid:(NSString *)uid ip:(NSString *)ip port:(NSNumber *)port;

@end

/**
 The properties of the event information monitored by the camera
 */
@interface CAMEventListInfo : NSObject

@property (strong, nonatomic) NSString *date;
@property (strong, nonatomic) NSNumber *status;
@property (strong, nonatomic) NSNumber *event;
@property (strong, nonatomic) NSNumber *subFileType;

+ (CAMEventListInfo *)initWithDate:(NSString *)date
                            status:(NSNumber *)status
                             event:(NSNumber *)event
                       subFileType:(NSNumber *)subFileType;

@end

@interface CAMClient : NSObject


+ (instancetype)shared;

- (dispatch_semaphore_t)getSemaphoreWith:(int)avIndex;

/**
 Search for cameras that are present in the LAN

 @param waitTimes Waiting time per search
 @param retryCount Number of searches
 @return A list of CAMCameraInfo
 */
- (NSMutableArray *)getLANCameraList:(int)waitTimes
                          RetryCount:(int)retryCount;

/**
 Connect to camera

 @param uID The UID of a device that client wants to connect
 @param name The view account for authentication,default is admin.
 @param password The view password for authentication,default is admin.
 @param channel The channel ID of the channel to start AV client
 @param complete A completion handler
 Error code if the value < 0
            - #AV_ER_INVALID_ARG The IOTC session ID is incorrect or reuse of IOTC session ID
 and IOTC channel ID.
             - #AV_ER_NOT_INITIALIZED AV module is not initialized yet
             - #AV_ER_EXCEED_MAX_CHANNEL The number of AV channels has reached maximum
             - #AV_ER_FAIL_CREATE_THREAD Fails to create threads
             - #AV_ER_SERV_NO_RESPONSE The AV server has no response
             - #AV_ER_SESSION_CLOSE_BY_REMOTE The remote site already closes
                 this IOTC session
             - #AV_ER_REMOTE_TIMEOUT_DISCONNECT The timeout expires because
                 remote site has no response.
             - #AV_ER_INVALID_SID The IOTC session ID is not valid
             - #AV_ER_CLIENT_EXIT Users stop this function with avClientExit() in another thread
             - #AV_ER_TIMEOUT The timeout specified by nTimeout expires before
                 AV start is performed completely
             - #AV_ER_WRONG_VIEWACCorPWD The client fails in authentication due
                 to incorrect view account or password
             - #AV_ER_NO_PERMISSION the AV module does not support Lite UID
             - #AV_ER_IOTC_CHANNEL_IN_USED the IOTC channel has been used by another av channel,
                 please check if the IOTC channel is used correctly
 
             - #IOTC_ER_NOT_INITIALIZED The IOTC module is not initialized yet
             - #IOTC_ER_UNLICENSE The specified UID of that device is not licensed or expired
             - #IOTC_ER_EXCEED_MAX_SESSION The number of IOTC sessions has reached maximum in client side
             - #IOTC_ER_DEVICE_NOT_LISTENING The device is not listening for connection now
             - #IOTC_ER_FAIL_CONNECT_SEARCH The client stop connecting to the device
             - #IOTC_ER_FAIL_RESOLVE_HOSTNAME Cannot resolve masters' host name
             - #IOTC_ER_FAIL_CREATE_THREAD Fails to create threads
             - #IOTC_ER_TCP_TRAVEL_FAILED Cannot connect to masters in neither UDP nor TCP
             - #IOTC_ER_TCP_CONNECT_TO_SERVER_FAILED Cannot connect to IOTC servers in TCP
             - #IOTC_ER_CAN_NOT_FIND_DEVICE IOTC servers cannot locate the specified device
             - #IOTC_ER_NO_PERMISSION The specified device does not support TCP relay
             - #IOTC_ER_SERVER_NOT_RESPONSE IOTC servers have no response
             - #IOTC_ER_FAIL_GET_LOCAL_IP Fails to get the client's local IP address
             - #IOTC_ER_FAIL_SETUP_RELAY Fails to connect the device via relay mode
             - #IOTC_ER_FAIL_CREATE_SOCKET Fails to create sockets
             - #IOTC_ER_FAIL_SOCKET_OPT Fails to set up socket options
             - #IOTC_ER_FAIL_SOCKET_BIND Fails to bind sockets
             - #IOTC_ER_NOT_SUPPORT_RELAY Not support relay connection by IOTC servers
             - #IOTC_ER_NO_SERVER_LIST No IOTC server information while client connect
             - #IOTC_ER_DEVICE_MULTI_LOGIN The connecting device duplicated loggin and may unconnectable
             - #IOTC_ER_MASTER_NOT_RESPONSE All masters have no respond
             - #IOTC_ER_DEVICE_IS_SLEEP Device is in sleep mode
             - #IOTC_ER_DEVICE_REJECT_BY_WRONG_AUTH_KEY connect with wrong key
 */
- (void)connectCameraWithUID:(NSString *)uID
                 accountName:(NSString *)name
             accountPassword:(NSString *)password
                     channel:(int) channel
                    complete:(kCAMConnectCameraBlock)complete;


/// Set the timeout period for connecting to the Camera, in seconds. It will affect the connectCameraWithUID command.
/// @param timeout The timeout for this function in unit of second
- (void)setConnectionTimeout:(NSTimeInterval)timeout;

/**
 Change AV Channel

 @param channel The channel ID of the channel to start AV client
 @param sid The session ID of the IOTC session to start AV client
 @param name The view account for authentication,default is admin.
 @param password The view password for authentication,default is admin.
 @return Error code if return value < 0
 - #AV_ER_INVALID_ARG The IOTC session ID is incorrect or reuse of IOTC session ID
 and IOTC channel ID.
 - #AV_ER_NOT_INITIALIZED AV module is not initialized yet
 - #AV_ER_EXCEED_MAX_CHANNEL The number of AV channels has reached maximum
 - #AV_ER_FAIL_CREATE_THREAD Fails to create threads
 - #AV_ER_SERV_NO_RESPONSE The AV server has no response
 - #AV_ER_SESSION_CLOSE_BY_REMOTE The remote site already closes
 this IOTC session
 - #AV_ER_REMOTE_TIMEOUT_DISCONNECT The timeout expires because
 remote site has no response.
 - #AV_ER_INVALID_SID The IOTC session ID is not valid
 - #AV_ER_CLIENT_EXIT Users stop this function with avClientExit() in another thread
 - #AV_ER_TIMEOUT The timeout specified by nTimeout expires before
 AV start is performed completely
 - #AV_ER_WRONG_VIEWACCorPWD The client fails in authentication due
 to incorrect view account or password
 - #AV_ER_NO_PERMISSION the AV module does not support Lite UID
 - #AV_ER_IOTC_CHANNEL_IN_USED the IOTC channel has been used by another av channel,
 please check if the IOTC channel is used correctly
 */

- (int)changeChannel:(int)channel
                 sid:(int)sid
         accountName:(NSString *)name
     accountPassword:(NSString *)password;

/**
 Stop an AV client

 @param sid The session ID of the IOTC session to close AV server
 @param avIndex The channel ID of the channel to close AV client
 */
//- (void)disconnectCmaeraWithIOTCModel:(int *)sid avIndex:(int)avIndex;

/**
 Stop an AV client
 
 @param avIndex The channel ID of the AV channel to be stopped
 */
- (void)closeChannel:(int)avIndex sid:(int)sid;


/**
 A device or a client uses this function to close a IOTC session
 specified by its session ID if this IOTC session is no longer
 required. If the closed SID is used for other APIs, the API should
 return IOTC_ER_INVALID_SID.
 
 @param sid The session ID of the IOTC session to be closed
 @return #IOTC_ER_NoERROR
 @return Error code if return value < 0
 - #IOTC_ER_NOT_INITIALIZED The IOTC module is not initialized yet
 - #IOTC_ER_INVALID_SID The specified IOTC session ID is not valid
 - #IOTC_ER_SESSION_CLOSE_BY_REMOTE The IOTC session of specified session ID has been closed by remote site
 - #IOTC_ER_REMOTE_TIMEOUT_DISCONNECT The timeout defined by #IOTC_SESSION_ALIVE_TIMEOUT
 expires because    remote site has no response
 */
- (int)closeSession:(int)sid;

/**
 Get the device is AP mode

 @param avIndex The channel ID of the AV client
 @return Wireless network type.
         0 : infrastructure
         2 : Host-AP
 */
- (int)getAPModelStatusWithAvIndex:(int) avIndex;



/**
 * \brief Send AV IO control
 *
 * \details This method is used to tell the Camera began to send video and audio streams.
 *
 * \param avIndex [in] The channel ID of the AV channel to send IO control
 * \return #AV_ER_NoERROR if sending successfully
 * \return Error code if return value < 0
 *            - #AV_ER_INVALID_ARG The AV channel ID is not valid
 *            - #AV_ER_SENDIOCTRL_ALREADY_CALLED This AV channel is already in sending
 *                IO control process
 *            - #AV_ER_SESSION_CLOSE_BY_REMOTE The remote site already closes
 *                this IOTC session
 *            - #AV_ER_REMOTE_TIMEOUT_DISCONNECT The timeout expires because
 *                remote site has no response.
 *            - #AV_ER_SENDIOCTRL_EXIT avSendIOCtrlExit() is called before this
 *                function is returned
 *            - #AV_ER_EXCEED_MAX_SIZE The IO control data and type to be sent exceeds
 *                #AV_MAX_IOCTRL_DATA_SIZE
 *            - #AV_ER_NO_PERMISSION the AV module does not support Lite UID
 *            - #AV_ER_IOTC_SESSION_CLOSED the local IOTC session has been closed
 *            - #AV_ER_IOTC_DEINITIALIZED IOTC has been deinitialized
 *          - #AV_ER_SERVER_EXIT avServStop() is called before this function is returned
 *          - #AV_ER_CLIENT_EXIT avClientStop() is called before this function is returned
 *
 * \see avRecvIOCtrl(), avSendIOCtrlExit()
 *
 * \attention This API is a blocking function and it will wait until
 *              having acknowledgment from the receiver.
 */
- (int)startIPCamStream:(int)avIndex;

/**
 * \brief Send AV IO control
 *
 * \details This method is used to tell the Camera to stop sending video and audio streams.
 *
 * \param avIndex [in] The channel ID of the AV channel to send IO control
 * \return #AV_ER_NoERROR if sending successfully
 * \return Error code if return value < 0
 *            - #AV_ER_INVALID_ARG The AV channel ID is not valid
 *            - #AV_ER_SENDIOCTRL_ALREADY_CALLED This AV channel is already in sending
 *                IO control process
 *            - #AV_ER_SESSION_CLOSE_BY_REMOTE The remote site already closes
 *                this IOTC session
 *            - #AV_ER_REMOTE_TIMEOUT_DISCONNECT The timeout expires because
 *                remote site has no response.
 *            - #AV_ER_SENDIOCTRL_EXIT avSendIOCtrlExit() is called before this
 *                function is returned
 *            - #AV_ER_EXCEED_MAX_SIZE The IO control data and type to be sent exceeds
 *                #AV_MAX_IOCTRL_DATA_SIZE
 *            - #AV_ER_NO_PERMISSION the AV module does not support Lite UID
 *            - #AV_ER_IOTC_SESSION_CLOSED the local IOTC session has been closed
 *            - #AV_ER_IOTC_DEINITIALIZED IOTC has been deinitialized
 *          - #AV_ER_SERVER_EXIT avServStop() is called before this function is returned
 *          - #AV_ER_CLIENT_EXIT avClientStop() is called before this function is returned
 *
 * \see avRecvIOCtrl(), avSendIOCtrlExit()
 *
 * \attention This API is a blocking function and it will wait until
 *              having acknowledgment from the receiver.
 */
- (int)stopIPCamStream:(int)avIndex;

/**
* \brief Send AV IO control
*
* \details This method is used to tell the Camera began to send video stream.
*
* \param avIndex [in] The channel ID of the AV channel to send IO control
* \return #AV_ER_NoERROR if sending successfully
* \return Error code if return value < 0
*            - #AV_ER_INVALID_ARG The AV channel ID is not valid
*            - #AV_ER_SENDIOCTRL_ALREADY_CALLED This AV channel is already in sending
*                IO control process
*            - #AV_ER_SESSION_CLOSE_BY_REMOTE The remote site already closes
*                this IOTC session
*            - #AV_ER_REMOTE_TIMEOUT_DISCONNECT The timeout expires because
*                remote site has no response.
*            - #AV_ER_SENDIOCTRL_EXIT avSendIOCtrlExit() is called before this
*                function is returned
*            - #AV_ER_EXCEED_MAX_SIZE The IO control data and type to be sent exceeds
*                #AV_MAX_IOCTRL_DATA_SIZE
*            - #AV_ER_NO_PERMISSION the AV module does not support Lite UID
*            - #AV_ER_IOTC_SESSION_CLOSED the local IOTC session has been closed
*            - #AV_ER_IOTC_DEINITIALIZED IOTC has been deinitialized
*          - #AV_ER_SERVER_EXIT avServStop() is called before this function is returned
*          - #AV_ER_CLIENT_EXIT avClientStop() is called before this function is returned
*
* \see avRecvIOCtrl(), avSendIOCtrlExit()
*
* \attention This API is a blocking function and it will wait until
*              having acknowledgment from the receiver.
*/
- (int)startVideoStream:(int)avIndex;

/**
* \brief Send AV IO control
*
* \details This method is used to tell the Camera to stop sending video stream.
*
* \param avIndex [in] The channel ID of the AV channel to send IO control
* \return #AV_ER_NoERROR if sending successfully
* \return Error code if return value < 0
*            - #AV_ER_INVALID_ARG The AV channel ID is not valid
*            - #AV_ER_SENDIOCTRL_ALREADY_CALLED This AV channel is already in sending
*                IO control process
*            - #AV_ER_SESSION_CLOSE_BY_REMOTE The remote site already closes
*                this IOTC session
*            - #AV_ER_REMOTE_TIMEOUT_DISCONNECT The timeout expires because
*                remote site has no response.
*            - #AV_ER_SENDIOCTRL_EXIT avSendIOCtrlExit() is called before this
*                function is returned
*            - #AV_ER_EXCEED_MAX_SIZE The IO control data and type to be sent exceeds
*                #AV_MAX_IOCTRL_DATA_SIZE
*            - #AV_ER_NO_PERMISSION the AV module does not support Lite UID
*            - #AV_ER_IOTC_SESSION_CLOSED the local IOTC session has been closed
*            - #AV_ER_IOTC_DEINITIALIZED IOTC has been deinitialized
*          - #AV_ER_SERVER_EXIT avServStop() is called before this function is returned
*          - #AV_ER_CLIENT_EXIT avClientStop() is called before this function is returned
*
* \see avRecvIOCtrl(), avSendIOCtrlExit()
*
* \attention This API is a blocking function and it will wait until
*              having acknowledgment from the receiver.
*/
- (int)stopVideoStream:(int)avIndex;
/**
* \brief Send AV IO control
*
* \details This method is used to tell the Camera began to send audio stream.
*
* \param avIndex [in] The channel ID of the AV channel to send IO control
* \return #AV_ER_NoERROR if sending successfully
* \return Error code if return value < 0
*            - #AV_ER_INVALID_ARG The AV channel ID is not valid
*            - #AV_ER_SENDIOCTRL_ALREADY_CALLED This AV channel is already in sending
*                IO control process
*            - #AV_ER_SESSION_CLOSE_BY_REMOTE The remote site already closes
*                this IOTC session
*            - #AV_ER_REMOTE_TIMEOUT_DISCONNECT The timeout expires because
*                remote site has no response.
*            - #AV_ER_SENDIOCTRL_EXIT avSendIOCtrlExit() is called before this
*                function is returned
*            - #AV_ER_EXCEED_MAX_SIZE The IO control data and type to be sent exceeds
*                #AV_MAX_IOCTRL_DATA_SIZE
*            - #AV_ER_NO_PERMISSION the AV module does not support Lite UID
*            - #AV_ER_IOTC_SESSION_CLOSED the local IOTC session has been closed
*            - #AV_ER_IOTC_DEINITIALIZED IOTC has been deinitialized
*          - #AV_ER_SERVER_EXIT avServStop() is called before this function is returned
*          - #AV_ER_CLIENT_EXIT avClientStop() is called before this function is returned
*
* \see avRecvIOCtrl(), avSendIOCtrlExit()
*
* \attention This API is a blocking function and it will wait until
*              having acknowledgment from the receiver.
*/
- (int)startAudioStream:(int)avIndex;

/**
* \brief Send AV IO control
*
* \details This method is used to tell the Camera to stop sending audio stream.
*
* \param avIndex [in] The channel ID of the AV channel to send IO control
* \return #AV_ER_NoERROR if sending successfully
* \return Error code if return value < 0
*            - #AV_ER_INVALID_ARG The AV channel ID is not valid
*            - #AV_ER_SENDIOCTRL_ALREADY_CALLED This AV channel is already in sending
*                IO control process
*            - #AV_ER_SESSION_CLOSE_BY_REMOTE The remote site already closes
*                this IOTC session
*            - #AV_ER_REMOTE_TIMEOUT_DISCONNECT The timeout expires because
*                remote site has no response.
*            - #AV_ER_SENDIOCTRL_EXIT avSendIOCtrlExit() is called before this
*                function is returned
*            - #AV_ER_EXCEED_MAX_SIZE The IO control data and type to be sent exceeds
*                #AV_MAX_IOCTRL_DATA_SIZE
*            - #AV_ER_NO_PERMISSION the AV module does not support Lite UID
*            - #AV_ER_IOTC_SESSION_CLOSED the local IOTC session has been closed
*            - #AV_ER_IOTC_DEINITIALIZED IOTC has been deinitialized
*          - #AV_ER_SERVER_EXIT avServStop() is called before this function is returned
*          - #AV_ER_CLIENT_EXIT avClientStop() is called before this function is returned
*
* \see avRecvIOCtrl(), avSendIOCtrlExit()
*
* \attention This API is a blocking function and it will wait until
*              having acknowledgment from the receiver.
*/
- (int)stopAudioStream:(int)avIndex;

/**
 Start receiving video streams

 @param tempAvIndex The channel ID of the AV channel to send IO control
 @param isPlayingBackVideo The receive video is play back video.
 @param receviedBlock Receive video callback.
 */
- (void)startReceiveVideo:(int)tempAvIndex
       isPlayingBackVideo:(BOOL)isPlayingBackVideo
                 received:(kCAMReceiveVideoBlock)receviedBlock;


/**
 Stop receiving video streams

 @param tempAvIndex The channel ID of the AV channel to send IO control
 */
- (void)stopReceiveVideo:(int)tempAvIndex;


/**
 Start receiving Audio streams

 @param tempAvIndex The channel ID of the AV channel to send IO control
 */
- (void)startReceiveAudio:(int)tempAvIndex;

/**
 Stop receiving Audio streams
 
 @param tempAvIndex The channel ID of the AV channel to send IO control
 */
- (void)stopReceiveAudio:(int)tempAvIndex;


/**
 Start Microphone

 @param sid The session ID of the IOTC session to create AV server
 @param avIndex The channel ID of the AV channel to send IO control
 @return #AV_ER_NoERROR if sending successfully
 @return Error code if return value < 0
             - #AV_ER_INVALID_ARG The AV channel ID is not valid
             - #AV_ER_SENDIOCTRL_ALREADY_CALLED This AV channel is already in sending
                 IO control process
             - #AV_ER_SESSION_CLOSE_BY_REMOTE The remote site already closes
                 this IOTC session
             - #AV_ER_REMOTE_TIMEOUT_DISCONNECT The timeout expires because
                 remote site has no response.
             - #AV_ER_SENDIOCTRL_EXIT avSendIOCtrlExit() is called before this
                 function is returned
             - #AV_ER_EXCEED_MAX_SIZE The IO control data and type to be sent exceeds
                 #AV_MAX_IOCTRL_DATA_SIZE
             - #AV_ER_NO_PERMISSION the AV module does not support Lite UID
             - #AV_ER_IOTC_SESSION_CLOSED the local IOTC session has been closed
             - #AV_ER_IOTC_DEINITIALIZED IOTC has been deinitialized
             - #AV_ER_SERVER_EXIT avServStop() is called before this function is returned
             - #AV_ER_CLIENT_EXIT avClientStop() is called before this function is returned
 */
- (int)startMicrophone:(int) sid
               avIndex:(int) avIndex;


/**
 Stop Microphone

 @param avIndex The channel ID of the AV channel to send IO control
 @param avServIndex The channel ID of the AV channel to be stopped
 @return #AV_ER_NoERROR if sending successfully
 @return Error code if return value < 0
        - #AV_ER_INVALID_ARG The AV channel ID is not valid
        - #AV_ER_SENDIOCTRL_ALREADY_CALLED This AV channel is already in sending
        IO control process
        - #AV_ER_SESSION_CLOSE_BY_REMOTE The remote site already closes
        this IOTC session
        - #AV_ER_REMOTE_TIMEOUT_DISCONNECT The timeout expires because
        remote site has no response.
        - #AV_ER_SENDIOCTRL_EXIT avSendIOCtrlExit() is called before this
        function is returned
        - #AV_ER_EXCEED_MAX_SIZE The IO control data and type to be sent exceeds
        #AV_MAX_IOCTRL_DATA_SIZE
        - #AV_ER_NO_PERMISSION the AV module does not support Lite UID
        - #AV_ER_IOTC_SESSION_CLOSED the local IOTC session has been closed
        - #AV_ER_IOTC_DEINITIALIZED IOTC has been deinitialized
        - #AV_ER_SERVER_EXIT avServStop() is called before this function is returned
        - #AV_ER_CLIENT_EXIT avClientStop() is called before this function is returned
 */
- (int)stopMicrophone:(int)avIndex
          avServIndex:(int)avServIndex;


/**
 Start send audio stream

 @param avIndex The channel ID of the AV channel to send IO control
 @param avServIndex The channel ID of the AV channel to send IO control
 */
- (void)startSendAudio:(int)avIndex
           avServIndex:(int)avServIndex;

/**
 Get event list based on start time and end time
 
 @param startDate Start time
 @param endDate End time
 @param eventID Event type
 @param avIndex The channel ID of the AV channel to send IO control.
 @return A list of CAMEventListInfo
 */
- (NSMutableArray *)getEventList:(NSDate *)startDate
                         endDate:(NSDate *)endDate
                         eventID:(CAMEventType)eventID
                         avIndex:(int)avIndex;

// MARK: Record

/**
 Start recording
 */
- (void)startRecord;

/**
 Stop recording

 @param complete A completion handler
 */
- (void)stopRecord:(kCAMConvertCompleteBlock) complete;

/**
 Get the length of the video

 @param recordDate Date of the video
 @param avIndex The channel ID of the AV channel to send IO control.
 @param sid The session ID of the IOTC session to send IO control.
 @return file play time (ms)
 */


/**
 Get the clarity of the corresponding channel

 @param channelIndex The channel index
 @param avIndex The channel ID of the AV channel to send IO control.
 @param completion A completion handler
 */
- (void)getResolutionInfo: (int)channelIndex
                  avIndex: (int)avIndex
          completionBlock: (kCAMSolutionBlock)completion;


/**
 Get the duration of the playback video

 @param recordDate Date of the video
 @param avIndex The channel ID of the AV channel to send IO control.
 @param sid The session ID of the IOTC session to send IO control.
 @return Duration of the playback video
 */
- (int)getPlaybackTimeWithRecordDate:(NSDate *)recordDate
                             avIndex:(int) avIndex
                                 sid:(int) sid;


/**
 Control the camera to start sending playback video streams
 @param accountName The view account for authentication,default is admin.
 @param password The view password for authentication,default is admin.
 @param recordDate Date of the video
 @param avIndex The channel ID of the AV channel to send IO control.
 @param sid The session ID of the IOTC session to send IO control.
 @return AV channel ID if return value >= 0
 @return Error code if return value < 0
             - #AV_ER_INVALID_ARG The IOTC session ID is incorrect or reuse of IOTC session ID
 and IOTC channel ID.
             - #AV_ER_NOT_INITIALIZED AV module is not initialized yet
             - #AV_ER_EXCEED_MAX_CHANNEL The number of AV channels has reached maximum
             - #AV_ER_FAIL_CREATE_THREAD Fails to create threads
             - #AV_ER_SERV_NO_RESPONSE The AV server has no response
             - #AV_ER_SESSION_CLOSE_BY_REMOTE The remote site already closes
                 this IOTC session
             - #AV_ER_REMOTE_TIMEOUT_DISCONNECT The timeout expires because
                 remote site has no response.
             - #AV_ER_INVALID_SID The IOTC session ID is not valid
             - #AV_ER_CLIENT_EXIT Users stop this function with avClientExit() in another thread
             - #AV_ER_TIMEOUT The timeout specified by nTimeout expires before
                 AV start is performed completely
             - #AV_ER_WRONG_VIEWACCorPWD The client fails in authentication due
                 to incorrect view account or password
             - #AV_ER_NO_PERMISSION the AV module does not support Lite UID
             - #AV_ER_IOTC_CHANNEL_IN_USED the IOTC channel has been used by another av channel,
                 please check if the IOTC channel is used correctly
 */
- (int)getPlaybackWithAccountName:(NSString *)accountName
                  accountPassword:(NSString *)password
                       recordDate:(NSDate *)recordDate
                          avIndex:(int) avIndex
                              sid:(int) sid;

/**
 Get the playback is finished

 @param avIndex The channel ID of the AV channel to send IO control.
 @return Success: 1(End)/0(Not end), fail: -1
 */
- (int)checkPlaybackEnd:(int) avIndex;


/**
 Pause playback

 @param recordDate Date of the video
 @param avIndex The channel ID of the AV channel to send IO control.
 @return Success if return value = 1
 */
- (int)pausePlaybackWithRecordDate:(NSDate *)recordDate
                           avIndex:(int) avIndex;

/**
 Stop playback

 @param recordDate Date of the video
 @param avIndex The channel ID of the AV channel to send IO control.
 @return Success if return value = 1
 */
- (int)stopPlaybackWithRecordDate:(NSDate *)recordDate
                          avIndex:(int) avIndex;



/**
 Start file transfer

 @param avIndex The channel ID of the AV channel to send IO control.
 @return The channel ID of the RDT channel.
 */
- (int)startFileTransfer:(int) avIndex;


/**
 Download event playback File

 @param sid The session ID of the IOTC session.
 @param avIndex The channel ID of the RDT channel
 @param eventDate RecordDate Date of the video
 @param filePath The directory where you want to save the file
 @param outFileName The file name where you want to save the file
 @param progressBlock Download progress
 @param finishBlock Finish Block
 */
- (void)startRDTClient:(int)sid
               avIndex:(int)avIndex
             EventDate:(NSString *)eventDate
              FilePath:(NSString *)filePath
              fileName:(NSString *)outFileName
      DownloadProgress:(kCAMDownloadVideoProgressBlock)progressBlock
        DownloadFinish:(kCAMDownloadVideoFinishBlock)finishBlock;

/// Save the camera snapshot to the specified file directory. You must use it after the "startReceiveVideo:isPlayingBackVideo:received:" command, otherwise you will receive false
/// @param filePath The file path where you want to save the snapshot
/// @return Success will return true
- (BOOL)saveSnapshotWithFilePath:(NSString *)filePath;

@end

NS_ASSUME_NONNULL_END
