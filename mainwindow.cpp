#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPushButton>
#include <QLineEdit>
#include <QGridLayout>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>
#include <QDialog>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QPushButton *btn = new QPushButton(this);
    QLineEdit *le = new QLineEdit(this);
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(btn, 0, 0, 1, 1);
    layout->addWidget(le, 0, 1, 1, 2);
    layout->addWidget(ui->textEdit, 1, 0, 1, 3);
    ui->centralwidget->setLayout(layout);

    isUntitled = true;
    curFile = tr("未命名.txt");
    setWindowTitle(curFile);

    findDlg = new QDialog(this);
    findDlg->setWindowTitle(tr("查找"));
    findLineEdit = new QLineEdit(findDlg);
    QPushButton *findBtn = new QPushButton(tr("查找下一个"), findDlg);
    QVBoxLayout *findLayout = new QVBoxLayout(findDlg);
    findLayout->addWidget(findLineEdit);
    findLayout->addWidget(findBtn);
    connect(findBtn, &QPushButton::clicked, this, &MainWindow::showFindText);

    // ui->statusbar->showMessage(tr("welcome to my Qt study project!"), 2000);
    statusLabel = new QLabel;
    statusLabel->setMinimumSize(150, 20);
    statusLabel->setFrameShape(QFrame::WinPanel);
    statusLabel->setFrameShadow(QFrame::Sunken);
    ui->statusbar->addWidget(statusLabel);
    statusLabel->setText(tr("welcome to my Qt study project!"));

    QLabel *permanent = new QLabel;
    permanent->setFrameStyle(QFrame::Box | QFrame::Sunken);
    permanent->setText(tr("<a href=\"https://www.csail.mit.edu\">MIT CSAIL</a>"));
    permanent->setTextFormat(Qt::RichText);
    permanent->setOpenExternalLinks(true);
    ui->statusbar->addPermanentWidget(permanent);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::newFile()
{
    if (maybeSave())
    {
        isUntitled = true;
        curFile = tr("未命名.txt");
        setWindowTitle(curFile);
        ui->textEdit->clear();
        ui->textEdit->setVisible(true);
    }
}

bool MainWindow::maybeSave()
{
    if (ui->textEdit->document()->isModified())
    {
        QMessageBox box;
        box.setWindowTitle(tr("警告"));
        box.setIcon(QMessageBox::Warning);
        box.setText(curFile + tr(" 尚未保存，是否保存?"));
        QPushButton *yesBtn = box.addButton(tr("是(&Y)"),
                                            QMessageBox::YesRole);
        box.addButton(tr("否(&N)"), QMessageBox::NoRole);
        QPushButton *cancelBut = box.addButton(tr("取消"),
                                            QMessageBox::RejectRole);
        box.exec();
        if (box.clickedButton() == yesBtn)
        {
            return save();
        }
        else if (box.clickedButton() == cancelBut)
        {
            return false;
        }
    }

    return true;
}

bool MainWindow::save()
{
    if (isUntitled)
    {
        return saveAs();
    }
    else
    {
        return saveFile(curFile);
    }
}

bool MainWindow::saveAs()
{
    QString filename = QFileDialog::getSaveFileName(this,
                                                    tr("另存为"), curFile);
    if (filename.isEmpty())
    {
        return false;
    }
    return saveFile(filename);
}

bool MainWindow::saveFile(const QString &fileName)
{
    QFile file(fileName);

    if (!file.open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox::warning(this, tr("多文档编辑器"),
                             tr("无法写入文件 %1: /n %2")
                             .arg(fileName).arg(file.errorString()));
        return false;
    }
    QTextStream out(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    out << ui->textEdit->toPlainText();
    QApplication::restoreOverrideCursor();
    isUntitled = false;
    curFile = QFileInfo(fileName).canonicalFilePath();
    setWindowTitle(curFile);

    return true;
}

bool MainWindow::loadFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this, tr("多文档编辑器"),
                             tr("无法读取文件 %1:\n%2.")
                             .arg(fileName).arg(file.errorString()));
        return false;
    }
    QTextStream in(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    ui->textEdit->setPlainText(in.readAll());
    QApplication::restoreOverrideCursor();

    curFile = QFileInfo(fileName).canonicalFilePath();
    setWindowTitle(curFile);

    return true;
}

void MainWindow::on_action_New_triggered()
{
    newFile();
}

void MainWindow::on_action_Save_triggered()
{
    save();
}

void MainWindow::on_action_SaveAs_triggered()
{
    saveAs();
}

void MainWindow::on_action_Open_triggered()
{
    if (maybeSave())
    {
        QString fileName = QFileDialog::getOpenFileName(this);
        if (!fileName.isEmpty())
        {
            loadFile(fileName);
            ui->textEdit->setVisible(true);
        }
    }
}

void MainWindow::on_action_Close_triggered()
{
    if (maybeSave())
    {
        ui->textEdit->setVisible(false);
    }
}

void MainWindow::on_action_Exit_triggered()
{
    on_action_Close_triggered();
    qApp->quit();
}

void MainWindow::on_action_Undo_triggered()
{
    ui->textEdit->undo();
}

void MainWindow::on_action_Cut_triggered()
{
    ui->textEdit->cut();
}

void MainWindow::on_action_Copy_triggered()
{
    ui->textEdit->copy();
}

void MainWindow::on_action_Paste_triggered()
{
    ui->textEdit->paste();
}

void MainWindow::on_action_Find_triggered()
{
    findDlg->show();
}

void MainWindow::showFindText()
{
    QString str = findLineEdit->text();
    if (!ui->textEdit->find(str, QTextDocument::FindBackward))
    {
        QMessageBox::warning(this, tr("查找"),
                             tr("找不到%1").arg(str));
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (maybeSave())
    {
        event->accept();
    }
    else
    {
        event->ignore();
    }
}
