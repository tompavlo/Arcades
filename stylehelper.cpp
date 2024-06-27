#include "stylehelper.h"


QString StyleHelper::getFieldWinStyle()
{
    return "QPushButton { "
        "background-color: #2e2e2e;"

    "border: 2px solid #555;"
        "font-size: 24px;"
        "font-weight: bold;"
    "color: #e0e0e0;"
        "min-width: 80px;"
        "min-height: 80px;"
    "}"

"QPushButton:hover {"
       " background-color: #3a3a3a;"
    "}"

"QPushButton:pressed {"
        "background-color: #1f1f1f;"
    "border: 2px solid #777;"
    "}"

"QPushButton:disabled {"
    "color:  rgb(235, 235, 235); "
        "background-color: #46963f;"

    "}";

}

QString StyleHelper::getFieldLostStyle()
{
    return "QPushButton { "
           "background-color: #2e2e2e;"

           "border: 2px solid #555;"
           "font-size: 24px;"
    "font-weight: bold;"
    "color: #e0e0e0;"
    "min-width: 80px;"
    "min-height: 80px;"
    "}"

    "QPushButton:hover {"
    " background-color: #3a3a3a;"
    "}"

    "QPushButton:pressed {"
    "background-color: #1f1f1f;"
    "border: 2px solid #777;"
    "}"

    "QPushButton:disabled {"
    "color:  rgb(235, 235, 235); "
    "background-color: #94251e;"

    "}";
}

QString StyleHelper::getLabelLostStyle()
{
    return "QLabel {";
        "background-color: #4a4a4a;"
    "border: 2px solid #555;"
        "border-radius: 10px;"
        "font-size: 20px;"
        "font-weight: bold;"
    "color: #e0e0e0;"
    "padding: 10px;"
        "min-width: 100px;"
        "min-height: 30px;"
        "text-align: center;"
    "}"

"QLabel:disabled {"
        "background-color: #4a4a4a;"
    "color: #888;"
        "}";
}

QString StyleHelper::getLabelWinStyle()
{
    return "QLabel {";
    "background-color: #46963f;"
    "border: 2px solid #555;"
    "border-radius: 10px;"
    "font-size: 20px;"
    "font-weight: bold;"
    "color: #e0e0e0;"
    "padding: 10px;"
    "min-width: 100px;"
    "min-height: 30px;"
    "text-align: center;"
    "}"

    "QLabel:disabled {"
    "background-color: #46963f;"
    "color: #888;"
    "}";
}

QString StyleHelper::getFielDefaultStyle()
{
    return "QPushButton { "
           "background-color: #2e2e2e;"
           "border: 2px solid #555;"
           "font-size: 24px;"
           "font-weight: bold;"
           "color: #e0e0e0;"
           "min-width: 80px;"
           "min-height: 80px;"
           "}"

           "QPushButton:hover {"
           "background-color: #3a3a3a;"
           "}"

           "QPushButton:pressed {"
           "background-color: #1f1f1f;"
           "border: 2px solid #777;"
           "}"

           "QPushButton:disabled {"
           "color: rgb(235, 235, 235);"
           "background-color: #4a4a4a;"
           "}";

}
