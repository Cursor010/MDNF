#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QMap>
#include <QMessageBox>
#include <QMovie>
#include <qmath.h>
#include <QRegularExpression>
#include <QMap>
#include <QStringList>
#include <QString>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void buildTable();
    Ui::MainWindow *ui;
    void certainMDNF();
    void findMDNF(int, QStringList, int, int);
    bool checkDuplicates(QStringList);
    bool isBanned(int, QString);
    bool isOverlapped(int, QString);
    QString getTerm(QString, QString);
    void updateCount(int);
    QStringList getSimpleColumn(QString);
    QStringList getColumn(QString);
    void setOriginalState();

signals:
    void mySignal();

private slots:
    void on_btnApply_clicked();
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void changeCellValue(int row, int col);

private:
    int cnt;
    int needOnes;
    int minLen;
    int cntP2;
    int currAction;
    QString currentFunction;
    QString varLbl;
    QMap <int, QStringList> headMap;
    QMap <int, QStringList> banned;
    QMap <int, QMap <QString, QList<int>>> accepted;
    QMap <int, int> selectedOnes;
    QMap <int, QMap <QString, bool>> selectedSets;
    QMap <QString, int> variableIndexMap;
    QMap <int, QList <QStringList>>answers;
    QTableWidget *mainTable;
    QStringList generateSubsets(int n);
    void fillHeadMap(QMap<int, QStringList> &headMap, int maxN);
};

#endif // MAINWINDOW_H
