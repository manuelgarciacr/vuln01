#include "./mainwindow.h"
#include "./ui_mainwindow.h"

#include <QMessageBox>
#include <string>
#include <QImage>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QCommandLinkButton>
#include <QEvent>
#include <QBuffer>
#include <QMimeDatabase>
#include <QDir>
#include <QFile>
#include <QSaveFile>

/**
 * Constructor of the class
 *
 * @brief MainWindow::MainWindow
 * @param parent
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)       // Parent class
    , ui(new Ui::MainWindow)    // Main window (User Interface)
    , url()                     // Image PNG File name. Passed as program argument
    , mimeType()                // Mime file type
    , code()
    , len()
    , base64()                  // Base64 encoded image

{
    // Initialize main window

    ui->setupUi(this);

    // Connect the "sendButton" button with its handler

    connect(ui->sendButton, &QCommandLinkButton::clicked, this, &MainWindow::sendButton_handler);
 }

/**
 * Class Destructor
 *
 * @brief MainWindow::~MainWindow
 */
MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * Loads data from the file name program argument
 *
 * @brief MainWindow::loadData
 * @param str Image PNG File name. Passed as program argument
 */
void MainWindow::loadData(std::string str)
{
    url = QString::fromUtf8(str);               // string to QString (object that provides a Unicode string)

    if (!QFile::exists(url))                    // If file not exists return
        return;

    mimeType = QMimeDatabase().mimeTypeForFile(url).name(); // Get file mime type

    if (mimeType != "image/png")                // If mime type is not png return
        return;

    QPixmap img = setWidgets(url);              // Sets the widgets on the screen

    QByteArray bArray = toByteArray(img);
    len = bArray.length();
qDebug() << "LEN" << len << "TBA" << bArray.toHex();
    memcpy(code, bArray.data(), len);

    //
    QByteArray dir{"\x7F\x39\x01\x32\x50"};
qDebug() << "DIR" << dir.length() << " " << dir.toHex();
    bArray.replace(1000, dir.length(), dir);

    // Save the data to a file.
    QSaveFile file("out.png");
    file.open(QIODevice::WriteOnly);
    file.write(bArray);
    file.commit(); // Commit is needed

    //QString codeQStr = toBase64(img);           // Encode the image
    //QByteArray code = codeQStr.toLocal8Bit();   // QString to QByteArray (required for strcpy and strcat commands)

    //QByteArray header = mimeType.toLocal8Bit();     // QString to QByteArray (object that provides an array of bytes)
    //header.prepend("data:").append(";base64,");     // Creates the header of the encoded string

    //strcpy(base64, header.data());              // Copy the header into the encoded string
    //strcat(base64, code.data());                // Append the code into the encoded string
}

/**
 * Load the PNG and the name of the file into the widgets on screen
 *
 * @brief MainWindow::loadImage
 * @param url Image PNG file name. Passed as program argument
 * @return the QPixmap with the loaded image
 */
QPixmap MainWindow::setWidgets(QString url) {

    // If the view 'graphicsView' in the UI doesn't have a scene, create it
    // It is a container for graphic items

    if (! ui->graphicsView->scene()) {
        QGraphicsScene *scene = new QGraphicsScene(this);
        ui->graphicsView->setScene(scene);
    }


    QPixmap img(url);                           // Load the image inside a QPixmap (object that contains off-screen image)
    QPixmap scaledImg = img.scaled(QSize(100, 100));          // Scale the image to the view size

    ui->graphicsView->scene()->addPixmap(scaledImg);  // Adds the image inside the scene of the view
    ui->label->setText(url);                    // Set the text in the label inside the UI

    return img;
}

/**
 * Encode image in a byte array
 *
 * @brief MainWindow::toBase64
 * @param pm Image loaded from the PNG file
 * @return Encoded image in a byte array
 */
QByteArray MainWindow::toByteArray(QPixmap pm) {
    QByteArray bArray;
    QBuffer buffer(&bArray);                // Create buffer
    buffer.open(QIODevice::WriteOnly);      // Open the buffer for write only
    QImage qi = pm.toImage();               // QPixmap to QImage (object that loads a hardware independent image)
    qi.save(&buffer, "PNG");                // QImage into the buffer with PNG format

    return bArray;                          // Returns the encoded buffer string
}

/**
 * Encode image in Base64
 *
 * @brief MainWindow::toBase64
 * @param pm Image loaded from the PNG file
 * @return Encoded image in a Base64 string
 */
QString MainWindow::toBase64(QPixmap pm) {
    QByteArray bArray;
    QBuffer buffer(&bArray);                // Create buffer
    buffer.open(QIODevice::WriteOnly);      // Open the buffer for write only
    QImage qi = pm.toImage();               // QPixmap to QImage (object that loads a hardware independent image)
    qi.save(&buffer, "BMP");                // QImage into the buffer with PNG format
qDebug() << "LEN" << buffer.data().toBase64().length();
    return buffer.data().toBase64();        // Returns the encoded buffer string
}

/**
 * Event (signals) handler for the sendButton
 *
 * @brief MainWindow::sendButton_handler
 */
void MainWindow::sendButton_handler()
{
    QMessageBox msgBox;                                         // Message box
    msgBox.setText(QString::fromUtf8(base64, strlen(base64)));  // Message  box textwith the encoded string
qDebug() << "B64 " << base64;                                         // Debug console message with the encoded string
   QByteArray bArray = QByteArray ( (const char *)code, len );
   //QByteArray bArray(QByteArray::fromRawData(code, len));
qDebug() << "BYTEARRAY " << bArray.toHex();
    msgBox.exec();                                              // Executes the message box
}
