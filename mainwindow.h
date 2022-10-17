#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);  // Constructor
    ~MainWindow();                          // Destructor
    void loadData(std::string str);         // Load data from file name program argument

private slots:                              // Event (signals) handlers
    void sendButton_handler();

private:
    Ui::MainWindow *ui;                     // Main window (User Interface)
    QString url;                            // Image PNG File name. Passed as program argument
    QString mimeType;                       // Mime file type
    std::byte code[23000];
    int len;
    char base64[41000];                     // Base64 encoded image
    QPixmap setWidgets(QString url);        // Load the PNG on screen
    QByteArray toByteArray(QPixmap pm);
    QString toBase64(QPixmap pm);           // Encode image in Base64

};
#endif // MAINWINDOW_H
