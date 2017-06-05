#include <jni.h>
#include <string>

#include <iostream>
#include <Poco/Net/HTTPSClientSession.h>
#include <Poco/Net/SSLException.h>
#include <Poco/Net/SSLManager.h>
#include <Poco/Net/AcceptCertificateHandler.h>
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
        Poco::Net::initializeSSL();
        const Poco::URI uri("https://github.com");
        const Poco::Net::Context::Ptr context(
                new Poco::Net::Context(Poco::Net::Context::CLIENT_USE, "",
                                       "","",Poco::Net::Context::VERIFY_RELAXED,
                                       9, true, "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH"));

        Poco::SharedPtr<Poco::Net::InvalidCertificateHandler> pAcceptCertHandler = new Poco::Net::AcceptCertificateHandler(
                false);
        Poco::Net::SSLManager::instance().initializeClient(NULL, pAcceptCertHandler, context);

        Poco::Net::HTTPSClientSession session(uri.getHost(), uri.getPort());
        Poco::Net::HTTPRequest req(Poco::Net::HTTPRequest::HTTP_GET, uri.getPath());
        req.setContentType("application/x-www-form-urlencoded;charset=UTF-8");
        req.set("USER-AGENT", "Mozilla/5.0 (Windows NT 6.1; WOW64; rv:35.0) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/46.0.2490.86 Safari/537.36");
//        req.setKeepAlive(true);

        session.sendRequest(req);
        Poco::Net::HTTPResponse res;
        std::istream &rs = session.receiveResponse(res);

        std::vector<Poco::Net::HTTPCookie> cookies;
        res.getCookies(cookies);


        StreamCopier::copyToString(rs, result);
    }
    catch (const Poco::Net::SSLException &e) {
        result = e.message();
    }
    catch (NetException &ex) {
        result = ex.displayText();
    }

//    std::string hello = "Hello from C++";
    return env->NewStringUTF(result.c_str());
}
