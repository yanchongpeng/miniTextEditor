#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QLineEdit;
class QDialog;
class QLabel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void newFile();
    bool maybeSave();
    bool save();
    bool saveAs();
    bool saveFile(const QString &fileName);
    bool loadFile(const QString &fileName);

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void on_action_New_triggered();

    void on_action_Save_triggered();

    void on_action_SaveAs_triggered();

    void on_action_Open_triggered();

    void on_action_Close_triggered();

    void on_action_Exit_triggered();

    void on_action_Undo_triggered();

    void on_action_Cut_triggered();

    void on_action_Copy_triggered();

    void on_action_Paste_triggered();

    void showFindText();

    void on_action_Find_triggered();

private:
    Ui::MainWindow *ui;
    bool isUntitled;
    QString curFile;

    QLineEdit *findLineEdit;
    QDialog *findDlg;

    QLabel *statusLabel;
};
#endif // MAINWINDOW_H
