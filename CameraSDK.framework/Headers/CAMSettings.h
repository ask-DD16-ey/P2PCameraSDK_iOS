//
//  CAMSettings.h
//  CameraSDK
//
//  Created by Arvin on 2019/6/26.
//  Copyright © 2019 Askey. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface CAMWiFiListInfo : NSObject
@property (strong, nonatomic) NSString *ssid;
@property (strong, nonatomic) NSNumber *mode;
@property (strong, nonatomic) NSNumber *enctype;
@property (strong, nonatomic) NSNumber *signal;
@property (strong, nonatomic) NSNumber *status;

+ (CAMWiFiListInfo *)initWithSSID:(NSString *)ssid
                           status:(NSNumber *)status
                          enctype:(NSNumber *)enctype
                           signal:(NSNumber *)signal
                             mode:(NSNumber *)mode;
@end

@interface CMADeviceInfo : NSObject
@property (strong, nonatomic) NSString *model;
@property (strong, nonatomic) NSNumber *channel;
@property (strong, nonatomic) NSNumber *free;
@property (strong, nonatomic) NSNumber *total;
@property (strong, nonatomic) NSString *vendor;
@property (strong, nonatomic) NSNumber *version;

+ (CMADeviceInfo *)initWithModel:(NSString *)model
                         channel:(NSNumber *)channel
                            free:(NSNumber *)free
                           total:(NSNumber *)total
                          vendor:(NSString *)vendor
                         version:(NSNumber *)version;
@end

@interface CAMSettingsInfo : NSObject
@property (assign, nonatomic) BOOL alwaysEnable;
@property (strong, nonatomic) NSString *deviceVender;
@property (strong, nonatomic) NSNumber *storageStatus;
@property (strong, nonatomic) NSNumber *videoMax;


+ (CAMSettingsInfo *)initWithDeviceVender:(NSString *)deviceVender
                             alwaysEnable:(BOOL) alwaysEnable
                            storageStatus:(NSNumber *)storageStatus
                                 videoMax:(NSNumber *)videoMax;
@end

@interface CAMSystemInfo : NSObject

@property (strong, nonatomic) NSString *gmt;
@property (strong, nonatomic) NSString *vender;
@property (strong, nonatomic) NSString *firmware;
@property (assign, nonatomic) int day;
@property (assign, nonatomic) int dst;
@property (assign, nonatomic) int hour;
@property (assign, nonatomic) int minute;
@property (assign, nonatomic) int second;
@property (assign, nonatomic) int month;
@property (assign, nonatomic) int year;
@property (assign, nonatomic) int simcard;
@property (assign, nonatomic) BOOL ledEnable;

@end

@interface CAMSensorDetailInfo : NSObject

@property (assign, nonatomic) int event1;
@property (assign, nonatomic) int event2;
@property (assign, nonatomic) int event3;
@property (assign, nonatomic) int motionValue;
@property (assign, nonatomic) int pirValue;
@property (assign, nonatomic) int soundValue;

@end

@interface CAMTimeZoneInfo : NSObject

@property (assign, nonatomic) int channel;
@property (assign, nonatomic) int day;
@property (assign, nonatomic) int dst;
@property (assign, nonatomic) int hour;
@property (assign, nonatomic) int minute;
@property (assign, nonatomic) int second;
@property (assign, nonatomic) int month;
@property (assign, nonatomic) int year;
@property (assign, nonatomic) int time_stamp;
@property (assign, nonatomic) int timezone;
@property (assign, nonatomic) int mask;

@end

@interface CAMFOTAResultInfo : NSObject

/*
 retult / reserved(int):
 -3 / 0 : download firmware fail
 -3 / 1 : check firmware fail
 -3 / 2 : upgreade firmware fail
 -2 / 0 : server error
 -1 / 0 : checking now
 0 / 0 : no newer firmware
 1 / version : get new firmware & version number
 2 / progress : downloading firmware & progress percentage 3 / 0 : check firmware size
 3 / 1 : check firmware MD5 checksum
 3 / 2 : check done.
 4 / progress : upgrading firmware & progress percentage
 5 / 0 : update success after bootup
 */
@property (assign, nonatomic) int result;
@property (assign, nonatomic) int reserved;

@end

@interface CAMSettings : NSObject

/**
 Get the WiFi information that camera can search for

 @param avIndex The channel ID of the channel.
 @return a list of CAMWiFiListInfo
 */
+ (NSMutableArray *)getWiFiList:(int) avIndex;


/**
 Connect to the specified WiFi

 @param wifiConfigDict wifi Info
 @param password WiFi password
 @param avIndex The channel ID of the channel.
 @return #AV_ER_NoERROR if sending successfully.If the password is entered incorrectly, the camera will enter ap mode.
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
+ (int)connectToWifi:(CAMWiFiListInfo *)wifiConfigDict
            password:(NSString *)password
             avIndex:(int) avIndex;


/**
 Get camera information

 @param avIndex The channel ID of the channel.
 @return Camera information
 */
+ (CMADeviceInfo *)getDeviceInfo:(int) avIndex;


/**
 Change Password

 @param oldPassword old password
 @param newPassword new password
 @param avIndex The channel ID of the channel.
 @return Modify success will return true
 */
+ (BOOL)setPassword:(NSString *)oldPassword
        newPassword:(NSString *)newPassword
            avIndex:(int) avIndex;


/**
 Get camera settings information

 @param avIndex The channel ID of the channel.
 @return return  Settings information
 */
+ (CAMSettingsInfo *)getSettingsInfo:(int) avIndex;


/**
 Get camera system information

 @param avIndex The channel ID of the channel.
 @return System information
 */
+ (CAMSystemInfo *)getSystemDetail:(int) avIndex;


/**
 Get camera sensor status information

 @param avIndex The channel ID of the channel.
 @return Sensor status information
 */
+ (CAMSensorDetailInfo *)getSensorDetail:(int) avIndex;


/**
 Get camera time zone information

 @param avIndex The channel ID of the channel.
 @return Time zone information
 */
+ (CAMTimeZoneInfo *)getSystemTimeDetail:(int) avIndex;


/**
 Restart the camera

 @param avIndex The channel ID of the channel.
 @return Success if return value = 1
 */
+ (int)setReboot:(int) avIndex;


/**
 Turn automatic recording on or off

 @param alwaysRecorder Turn on or off
 @param avIndex The channel ID of the channel.
 @return #AV_ER_NoERROR if sending successfully.
 */
+ (int)setAlwaysRecorder:(BOOL)alwaysRecorder
                 avIndex:(int) avIndex;


/**
 Set the maximum time for automatic recording,Turn automatic recording on or off

 @param maxLength the maximum time for automatic recording,need between 10 and 3600
 @param alwaysRecorder Turn on or off
 @param avIndex The channel ID of the channel.
 @return #AV_ER_NoERROR if sending successfully.
 */
+ (int)setRecorderMaxLength:(int)maxLength
             AlwaysRecorder:(BOOL)alwaysRecorder
                    avIndex:(int) avIndex;


/**
 Set camera system time zone

 @param timeZone Time zone string,this value requires URL encoding
 Example: +04:00 >> %2B04%3A00      -01:00 >> -01%3A00
 @param dst Whether to turn on Daylight
 @param avIndex The channel ID of the channel.
 @return #AV_ER_NoERROR if sending successfully.
 */
+ (int)setSystemTimeZone:(NSString *)timeZone
          DaylightEnable: (BOOL)dst
                 avIndex:(int) avIndex;


/**
 Format SD card

 @param avIndex The channel ID of the channel.
 @return #AV_ER_NoERROR if sending successfully.
 */
+ (int)formatSDCard:(int) avIndex;

/**
 Get evnet setting

 @param avIndex The channel ID of the channel.
 @return ENUM_EVENT_STATUS bit0 : motion 0(disabled) / 1(enabled)
                           bit1 : PIR 0(disabled) / 1(enabled)
                           bit2 : audio 0(disabled) / 1(enabled)
 */
+ (int)getEventStatus:(int) avIndex;


/**
 Set evnet setting

 @param status ENUM_EVENT_STATUS
 bit0 : motion 0(disabled) / 1(enabled)
 bit1 : PIR 0(disabled) / 1(enabled)
 bit2 : audio 0(disabled) / 1(enabled)
 @param avIndex The channel ID of the channel.
 @return #AV_ER_NoERROR if sending successfully.
 */
+ (int)setEvent:(int)status
        avIndex:(int) avIndex;


/**
 Get motion detection sensitivity
 @param avIndex The channel ID of the channel.
 @return sensitivity value
 0(Disabled) ~ 100(MAX):
 index        sensitivity value
 0            0
 1            25
 2            50
 3            75
 4            100
 */
+ (int)getMDsensitivity:(int) avIndex;


/**
 Set motion detection sensitivity

 @param sensitivity sensitivity value
 @param avIndex The channel ID of the channel.
 @return #AV_ER_NoERROR if sending successfully.
 */
+ (int)setMDsensitivity:(int) sensitivity
                avIndex:(int) avIndex;


/**
 Turn LED on or off

 @param isOn Turn on or off
 @param avIndex The channel ID of the channel.
 @return return 1 if sending successfully.
 */
+ (int)changeLEDStatus:(BOOL)isOn avIndex:(int) avIndex;


/**
 Tell camera to start check firmware version

 @param avIndex The channel ID of the channel.
 @return #AV_ER_NoERROR if sending successfully.
 */
+ (int)checkVersion:(int)avIndex;


/**
 Get camera check firmware version result, download progress, install progress

 @param avIndex The channel ID of the channel.
 @return CAMFOTAResultInfo
     retult / reserved(int):
     -3 / 0 : download firmware fail
     -3 / 1 : check firmware fail
     -3 / 2 : upgreade firmware fail
     -2 / 0 : server error
     -1 / 0 : checking now
     0 / 0 : no newer firmware
     1 / version : get new firmware & version number
     2 / progress : downloading firmware & progress percentage 3 / 0 : check firmware size
     3 / 1 : check firmware MD5 checksum
     3 / 2 : check done.
     4 / progress : upgrading firmware & progress percentage
     5 / 0 : update success after bootup
 */
+ (CAMFOTAResultInfo *)getFOTAResult:(int) avIndex;


/**
 Tell camera to start download new firmware

 @param avIndex The channel ID of the channel.
 @return Error code for value = -1, success for value = 0
 */
+ (int)downloadVersion:(int) avIndex;


/**
 Tell camera to start install new firmware

 @param avIndex The channel ID of the channel.
 @return Error code for value = -1, success for value = 0
 */
+ (int)installVersion:(int) avIndex;


/**
 Send json string to set or get camera configuration

 @param avIndex The channel ID of the AV channel to be sent
 @param jsonString Json string for camera settings
 @return A Dictionary of camera settings, if send command failed，return errorCode, if success return  [Response Json String]
 */
+ (id)cameraSetting:(int)avIndex
         JsonString:(NSString *)jsonString;

/// Get the current version number of the CameraSDK
+ (NSString *)getVersion;

@end

NS_ASSUME_NONNULL_END
