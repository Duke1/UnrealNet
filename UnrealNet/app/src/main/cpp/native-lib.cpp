#include <jni.h>
#include <string>

#include <iostream>
#include <Poco/Net/HTTPSClientSession.h>
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/StreamCopier.h"
#include "Poco/Net/NetException.h"
#include "Poco/Net/HTMLForm.h"
#include "Poco/URI.h"

using namespace Poco;
using namespace Net;


extern "C"
JNIEXPORT jstring JNICALL
Java_xyz_openhh_unrealnet_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {

    std::string result;
    try {
        URI url("https:://github.com");
        HTTPSClientSession session(url.getHost(), url.getPort());
        HTTPRequest req(HTTPRequest::HTTP_GET, HTTPRequest::HTTP_1_1);
//        req.set("/s?word=", "C++");
        session.sendRequest(req);
        HTTPResponse res;
        std::istream &is = session.receiveResponse(res);
        StreamCopier::copyToString(is, result);
    }
    catch (NetException &ex) {
        result = ex.displayText();
    }

//    std::string hello = "Hello from C++";
    return env->NewStringUTF(result.c_str());
}
