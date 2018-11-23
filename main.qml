import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.0

Window {
    visible: true
    width: Screen.width
    height: Screen.height
    title: qsTr("Update and Recovery")
    property int fontSize: 24;
    property var showColor: ["gray","#4b9cfa"];
    property int timeCount: 0;
    Rectangle{
        anchors.fill: parent;
        Image{
            id: symble
            anchors{
                horizontalCenter: parent.horizontalCenter;
                bottom: message.top;
                bottomMargin: 10;
            }
            source: "qrc:/images/updateSymble.png";
        }
        Label{
            id: message;
            anchors.centerIn: parent;
            font{
                pixelSize: fontSize;
                family: "Tahoma";
            }
            height: 34;
            color: "#333333";
            text: upgrader.message;
            //text: "message";
        }
        Rectangle{
            id: progress;
            //visible: marqueeTimer.running
            width: 144;
            anchors{
                top: message.bottom;
                horizontalCenter: parent.horizontalCenter;
                topMargin: 10;
            }
            Row{
                spacing: 10;
                Rectangle{
                    id: point1;
                    width: 24; height: 24; radius: width / 2;
                    color: showColor[timeCount >= 1 ? 1 : 0];//"#4b9cfa";
                    anchors.verticalCenter: parent.verticalCenter;
                }
                Rectangle{
                    id: point2;
                    width: 24; height: 24; radius: width / 2;
                    color: showColor[timeCount >= 2 ? 1 : 0];//"#4b9cfa";
                    anchors.verticalCenter: parent.verticalCenter;
                }
                Rectangle{
                    id: point3;
                    width: 24; height: 24; radius: width / 2;
                    color: showColor[timeCount >= 3 ? 1 : 0];
                    anchors.verticalCenter: parent.verticalCenter;
                }
                Rectangle{
                    width: 12;height: 12;
                    color: "transparent";
                }
                Label{
                    id: progressValue
                    font{
                        pixelSize : fontSize;
                        family : "Tahoma";
                    }
                    verticalAlignment: Text.AlignVCenter
                    color : "#333333";
                    text: upgrader.progress + "%";
                    //text: "80%";
                }

            }
        }
        Timer{
            id: marqueeTimer;
            interval: 1000;
            repeat: true;
            triggeredOnStart: true;
            onTriggered:{
                timeCount = (timeCount + 1) % 4;
            }
        }
        Connections{
            target: upgrader
            onMarqueeStart:{
                marqueeTimer.start();
            }
            onMarqueeFinish:{
                marqueeTimer.stop();
                timeCount = 0;
            }
        }
        Component.onCompleted: {
            upgrader.start();
            //marqueeTimer.start();
        }
    }
}
