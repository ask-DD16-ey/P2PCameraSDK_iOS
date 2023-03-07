//
//  CAMAudio.h
//  CameraSDK
//
//  Created by Arvin on 2019/6/24.
//  Copyright © 2019 Askey. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>
#import <AudioToolbox/AudioToolbox.h>

NS_ASSUME_NONNULL_BEGIN

@interface CAMAudio : NSObject 

@property (strong, nonatomic) NSMutableData *audioDataSave;
@property (strong, nonatomic) NSMutableData *audioDataSendSave;
@property (assign, nonatomic) BOOL isAudioMute;
@property (assign, nonatomic) BOOL isMicPhoneOpen;

+ (instancetype)shared;


/**
 Call this api to stop playing audio from camera
 */
- (void)stopAudio;


/**
 Call this api to ready for playing audio from camera
 */
- (void)startAudio;


/**
 Stop microphone in iPhone to stop to send audio data to camera.
 */
- (void)stopMicPhone;


/**
 Open microphone in iPhone to ready for sending audio data to camera.
 */
- (void)startMicPhone;

@end
NS_ASSUME_NONNULL_END
