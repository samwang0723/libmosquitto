libmosquitto
============

This is an XCode project skeleton structure that can be used to build a static 
libmosquitto.a library for use in Mac and iPhone projects.

Built from these sources: http://mosquitto.org/files/source/mosquitto-1.1.3.tar.gz

Usage
-----
Build using XCode, TARGET -> UniversalLib

    - (void) mqttInit:(NSString *)host withPort:(int)port
    {
        // mosquitto ssl client connection
        NSString *clientId = @"marquette_sample";
        NSLog(@"Client ID: %@", clientId);
        mMosquittoClient = [[MosquittoClient alloc] initWithClientId:clientId];
        [mMosquittoClient setDelegate:self];
        [mMosquittoClient setHost:host];
        [mMosquittoClient setPort:port];
        [mMosquittoClient connect];
    }

    - (void) mqttInitWithSSL:(NSString *)host withPort:(int)port
    {    
        NSBundle *mainBundle = [NSBundle mainBundle];
        NSString *caCrtFile = [mainBundle pathForResource: @"ca" ofType: @"crt"];
        NSLog(@"caCrtFile=%@", caCrtFile);
        const char *caCrt = [caCrtFile cStringUsingEncoding:NSASCIIStringEncoding];

        NSString *clientCrtFile = [mainBundle pathForResource: @"client" ofType: @"crt"];
        const char *clientCrt = [clientCrtFile cStringUsingEncoding:NSASCIIStringEncoding];

        NSString *clientKeyFile = [mainBundle pathForResource: @"client" ofType: @"key"];
        const char *clientKey = [clientKeyFile cStringUsingEncoding:NSASCIIStringEncoding];

        // mosquitto ssl client connection
        NSString *clientId = @"marquette_sample";
        NSLog(@"Client ID: %@", clientId);
        mMosquittoClient = [[MosquittoClient alloc] initWithClientId:clientId];
        [mMosquittoClient setDelegate:self];
        [mMosquittoClient setHost:host];
        [mMosquittoClient setPort:port];
        [MosquittoClient setClientPassword:@"client"];
        [mMosquittoClient connectWithSSL:TLSV1 caCrt:caCrt caLocation:NULL clientCrt:clientCrt clientKey:clientKey];
    }

    - (void) mqttSubscribe:(NSString *)topic withQos:(int)qos
    {
        if(nil != mMosquittoClient){
            [mMosquittoClient subscribe:topic withQos:qos];
        }
    }

    - (void) mqttPublish:(NSString *)topic withMessage:(NSString *)message withQos:(int)qos;
    {
        if(nil != mMosquittoClient){
            NSUInteger nsQos = qos;
            [mMosquittoClient publishString:message toTopic:topic withQos:nsQos retain:YES];
        }
    }

    - (void) mqttDisconnect
    {
        if(nil != mMosquittoClient){
            [mMosquittoClient disconnect];
        }
    }

    - (void) mqttReconnect
    {
        NSLog(@"mqttReconnect");
        if(nil != mMosquittoClient){
            [mMosquittoClient reconnect];
        }
    }

    // Mosquitto callback listener
    - (void) didConnect:(NSUInteger)code
    {
        NSLog(@"mosquitto didConnect");
        if(mTimer != nil){
            [mTimer invalidate];
            mTimer = nil;
        }
        [self mqttSubscribe:@"test" withQos:1];
        //[self mqttPublish:@"test" withMessage:@"sample string" withQos:1];
    }

    - (void) didDisconnect
    {
        NSLog(@"mosquitto didDisconnect");

        mTimer = [NSTimer scheduledTimerWithTimeInterval:10 // 10sec
            target:self
            selector:@selector(mqttReconnect)
            userInfo:nil
            repeats:YES];
    }

    - (void) didReceiveMessage:(MosquittoMessage*) mosq_msg
    {
        NSLog(@"%@ => %@", mosq_msg.topic, mosq_msg.payload);
    }

    - (void) didPublish: (NSUInteger)messageId {}
    - (void) didSubscribe: (NSUInteger)messageId grantedQos:(NSArray*)qos {}
    - (void) didUnsubscribe: (NSUInteger)messageId {}


TODO

License
-------

Copyright (c) 2009-2014 Sam Wang  <sam.wang.0723@gmail.com>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.
3. Neither the name of mosquitto nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
