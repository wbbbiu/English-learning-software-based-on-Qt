import QtQuick 2.15
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.15
Rectangle{
    anchors.fill: parent
        Image {
            id: welcome_image
            anchors.fill: parent
            source: "qrc:/image/image/welcome.jpg"
            onStatusChanged: {
                      if (status === Image.Ready) {
                          console.log("Image loaded successfully")
                      } else if (status === Image.Error) {
                          console.log("Error loading image:", source)
                      }
        }

}
}

