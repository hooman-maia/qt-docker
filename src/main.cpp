#include <QCoreApplication>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QEventLoop>
#include <QDebug>

const QString baseUrl = "http://host.docker.internal:3000";

QNetworkAccessManager* getManager() {
    static QNetworkAccessManager manager;
    return &manager;
}

void waitForReply(QNetworkReply* reply) {
    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() == QNetworkReply::NoError) {
        QByteArray response = reply->readAll();
        qDebug() << "Response:" << response;
    } else {
        qDebug() << "Error:" << reply->errorString();
    }

    reply->deleteLater();
}

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    // Test code
    // -----------------------
QUrl url(baseUrl + "/api/cases");
QNetworkRequest request(url);
request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

QJsonObject jsonPayload;
jsonPayload["surgeryBeginning"] = "2023-07-09T10:00:00Z";
jsonPayload["surgeryEnding"] = "2023-07-09T12:00:00Z";
jsonPayload["studyInstanceUID"] = "1.2.3.4.5.6.7.8.9";

QJsonObject patient;
QJsonObject patientName;
patientName["givenName"] = "John";
patientName["familyName"] = "Doe";
patient["patientName"] = patientName;
patient["patientBirthDate"] = "1980-01-01";
patient["patientID"] = "P12345";
jsonPayload["patient"] = patient;

QJsonObject performingPhysician;
QJsonObject physicianName;
physicianName["givenName"] = "Jane";
physicianName["familyName"] = "Smith";
performingPhysician["performingPhysicianName"] = physicianName;
jsonPayload["performingPhysician"] = performingPhysician;

jsonPayload["niftiImage"] = "path/to/image.nii";
jsonPayload["isInProgress"] = true;

QJsonDocument doc(jsonPayload);
QByteArray data = doc.toJson();

QNetworkAccessManager manager;
QNetworkReply *reply = manager.post(request, data);

QObject::connect(reply, &QNetworkReply::finished, [=]() {
    QByteArray response = reply->readAll();
    if (reply->error() == QNetworkReply::NoError) {
        qDebug() << "Response:" << response;
    } else {
        qDebug() << "Error:" << reply->errorString();
        qDebug() << "Error details:" << response;
    }
    reply->deleteLater();
});

    return a.exec();
}



// Second Example with a multi-method function for request


// void sendRequest(const QUrl &url, const QString &method, const QJsonObject &data = QJsonObject()) {
//     QNetworkAccessManager manager;
//     QNetworkRequest request(url);
//     request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

//     QNetworkReply *reply;
//     if (method == "GET") {
//         reply = manager.get(request);
//     } else if (method == "POST") {
//         reply = manager.post(request, QJsonDocument(data).toJson());
//     } else if (method == "PUT") {
//         reply = manager.put(request, QJsonDocument(data).toJson());
//     } else if (method == "PATCH") {
//         reply = manager.sendCustomRequest(request, "PATCH", QJsonDocument(data).toJson());
//     } else if (method == "DELETE") {
//         reply = manager.deleteResource(request);
//     } else {
//         qDebug() << "Unsupported method:" << method;
//         return;
//     }

//     QEventLoop eventLoop;
//     QObject::connect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
//     eventLoop.exec();

//     if (reply->error() == QNetworkReply::NoError) {
//         QByteArray response_data = reply->readAll();
//         qDebug() << method << "Response:" << response_data;
//     } else {
//         qDebug() << method << "Error:" << reply->errorString();
//     }

//     reply->deleteLater();
// }

// int main(int argc, char *argv[]) {
//     QCoreApplication a(argc, argv);

//     // Test create new session
//     qDebug() << "Creating new session:";
//     sendRequest(QUrl(baseUrl + "/api/sessions"), "POST");

//     // Test get current session status
//     qDebug() << "\nGetting current session status:";
//     sendRequest(QUrl(baseUrl + "/api/sessions/current"), "GET");

//     // Test deactivate all sessions
//     qDebug() << "\nDeactivating all sessions:";
//     sendRequest(QUrl(baseUrl + "/api/sessions/deactivate"), "POST");

//     return a.exec();
// }