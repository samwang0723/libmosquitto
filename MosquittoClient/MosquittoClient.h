//
//  MosquittoClient.h
//
//  Copyright 2012 Nicholas Humfrey. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MosquittoMessage.h"

#define TLSV1 "tlsv1"
#define TLSV1_1 "tlsv1.1"
#define TLSV1_2 "tlsv1.2"

@protocol MosquittoClientDelegate

- (void) didConnect: (NSUInteger)code;
- (void) didDisconnect;
- (void) didPublish: (NSUInteger)messageId;

- (void) didReceiveMessage: (MosquittoMessage*)mosq_msg;
- (void) didSubscribe: (NSUInteger)messageId grantedQos:(NSArray*)qos;
- (void) didUnsubscribe: (NSUInteger)messageId;

@end


@interface MosquittoClient : NSObject {
    struct mosquitto *mosq;
    NSString *host;
    unsigned short port;
    NSString *username;
    NSString *password;
    unsigned short keepAlive;
    BOOL cleanSession;
    
    id<MosquittoClientDelegate> delegate;
    NSTimer *timer;
}

@property (readwrite,retain) NSString *host;
@property (readwrite,assign) unsigned short port;
@property (readwrite,retain) NSString *username;
@property (readwrite,retain) NSString *password;
@property (readwrite,assign) unsigned short keepAlive;
@property (readwrite,assign) BOOL cleanSession;
@property (readwrite,assign) id<MosquittoClientDelegate> delegate;

+ (void) initialize;
+ (NSString*) version;

- (MosquittoClient*) initWithClientId: (NSString*) clientId;
- (MosquittoClient*) initWithClientId: (NSString*) clientId userName:(NSString *)userName password:(NSString *)password;
- (void) setMessageRetry: (NSUInteger)seconds;
- (void) connect;
- (void) connectToHost: (NSString*)host;
- (void) reconnect;
- (void) disconnect;

- (void)setWill: (NSString *)payload toTopic:(NSString *)willTopic withQos:(NSUInteger)willQos retain:(BOOL)retain;
- (void)clearWill;

- (void)publishString: (NSString *)payload toTopic:(NSString *)topic withQos:(NSUInteger)qos retain:(BOOL)retain;

- (void)subscribe: (NSString *)topic;
- (void)subscribe: (NSString *)topic withQos:(NSUInteger)qos;
- (void)unsubscribe: (NSString *)topic;


// This is called automatically when connected
- (void) loop: (NSTimer *)timer;

// @Sam_Wang, for SSL connection purpose only
+ (void) setClientPassword:(NSString *)password;
- (void) connectWithSSL:(const char *)tlsVer caCrt:(const char *)caCrt caLocation:(const char *)caLocation clientCrt:(const char *)clientCrt clientKey:(const char *)clientKey;

//@Sam_Wang, add for cleaning mosquitto_lib
- (void)clearMosquittoLib;

@end
