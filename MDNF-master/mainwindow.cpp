#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), cnt(4), needOnes(0), minLen(1000000000), cntP2(16), currAction(0),
    currentFunction("00000000"), varLbl("abcdef") {
    mainTable = new QTableWidget(this);
    ui->setupUi(this);
    fillHeadMap(headMap, 6);
    this->showMaximized();
    ui->funcNumberInput2_2->setText("4");
    QObject::connect(ui->mainTable, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(changeCellValue(int, int)));
}

MainWindow::~MainWindow() {
    delete mainTable;
    delete ui;
}

void MainWindow::changeCellValue(int row, int col) {
    if (col != 0 || currAction != 0)return;
    QTableWidgetItem * item = ui->mainTable->item(row,col);
    if (item->text() == "0"){
        item->setText("1");
    } else {
        item->setText("0");
    }
    QString binaryValue;
    for (int r = 0; r < ui->mainTable->rowCount(); ++r) {
        QTableWidgetItem *currentItem = ui->mainTable->item(r, col);
        if (currentItem) {
            binaryValue += currentItem->text();
        }
    }
    currentFunction = binaryValue;
    bool ok;
    long long decimalValue = currentFunction.toLongLong(&ok, 2);
    QString decimalStr = QString::number(decimalValue);
    ui->funcNumberInput2->setText(decimalStr);
    buildTable();
}

bool MainWindow::isBanned(int x, QString s) {
    return banned[x].indexOf(s) != -1;
}

bool MainWindow::isOverlapped(int col, QString set) {
    for (int i = 0; i < accepted[col][set].length(); ++i) {
        if (!selectedOnes[accepted[col][set][i]]) {
            return false;
        }
    }
    return true;
}

void MainWindow::setOriginalState() {
    mainTable = new QTableWidget(this);
    ui->lblAns->clear();
    minLen = 1000000000;
    currAction = 0;
    answers.clear();
    needOnes = 0;
    selectedOnes.clear();
    selectedSets.clear();
    accepted.clear();
    banned.clear();
}

bool MainWindow::checkDuplicates(QStringList list) {
    int flag;
    for (int i = 0; i < answers[minLen].length(); ++i) {
        flag = 1;
        for (int j = 0; j < list.length(); ++j) {
            if (answers[minLen][i][j] != list[j]) {
                flag = 0;
            }
        }
        if (flag) return false;
    }
    return true;
}

QString MainWindow::getTerm(QString head, QString val) {
    QString ans;
    for (int i = 0; i < val.length(); ++i) {
        if(val[i] == '1') {
            ans += '~' + head[i];
        } else {
            ans += head[i];
        }
    }
    return ans;
}

void MainWindow::updateCount(int x) {
    cnt = x;
    cntP2 = pow(2, x);
    for(int i = 0; i < x; ++i) {
        variableIndexMap[varLbl[i]] = x - i - 1;
    }
}

QStringList MainWindow::getSimpleColumn(QString s) {
    int x = variableIndexMap[s];
    QStringList ans;
    while(ans.length() < cntP2) {
        for (int i = 0; i < pow(2, x); ++i) {
            ans << "0";
        }
        for(int i = 0; i < pow(2, x); ++i) {
            ans << "1";
        }
    }
    return ans;
}

QStringList MainWindow::getColumn(QString s) {
    QStringList ans;
    for (int i = 0; i < cntP2; ++i) {
        ans << "";
    }
    for(int i = 0; i < s.length(); ++i) {
        QStringList tmpColumn = getSimpleColumn(QString(s[i]));
        for (int j = 0; j < cntP2; ++j) {
            ans[j] += tmpColumn[j];
        }
    }
    return ans;
}

void MainWindow::buildTable() {
    mainTable = ui->mainTable;
    int tmpColumnCount = mainTable->columnCount();
    for(int i = 0; i < tmpColumnCount; ++i) {
        mainTable->removeColumn(0);
        mainTable->removeRow(0);
    }
    for (int i = 0; i < cntP2; ++i) {
        mainTable->insertRow(i);
        mainTable->insertColumn(i);
    }
    QTableWidgetItem * item = new QTableWidgetItem();
    item->setText("f");
    mainTable->setHorizontalHeaderItem(0, item);
    for(int i = 0; i < cntP2; ++i) {
        QTableWidgetItem * itm = new QTableWidgetItem();
        itm->setText(QString(currentFunction[i]));
        itm->setFlags(Qt::ItemIsEnabled);
        itm->setTextAlignment(Qt::AlignCenter);
        mainTable->setItem(i, 0, itm);
    }
    for (int i = 1; i < cntP2; ++i) {
        QTableWidgetItem * itm = new QTableWidgetItem();
        itm->setText(headMap[cnt][i - 1]);
        mainTable->setHorizontalHeaderItem(i, itm);
    }
    for (int j = 1; j < cntP2; ++j) {
        QString hed = headMap[cnt][j - 1];
        QStringList column = getColumn(hed);
        for (int i = 0; i < column.length(); ++i) {
            QCoreApplication::processEvents();
            QTableWidgetItem * itm = new QTableWidgetItem();
            itm->setText(column[i]);
            itm->setFlags(Qt::ItemIsEnabled);
            itm->setTextAlignment(Qt::AlignCenter);
            mainTable->setItem(i, j, itm);
        }
    }
}

void MainWindow::findMDNF(int curLen, QStringList curDNF, int idx, int ones) {
    if(ones == needOnes) {
        if (curLen <= minLen) {
            minLen = curLen;
            curDNF.sort();
            if (checkDuplicates(curDNF)) {
                answers[curLen] << curDNF;
            }
        }
        return;
    }
    QCoreApplication::processEvents();
    int tmpIdx = 0;
    for (int i = 1; i < cntP2; ++i) {
        foreach (QString key, accepted[i].keys()) {
            if(tmpIdx++ < idx) continue;
            if(isOverlapped(i, key)) continue;
            if(curLen+key.length() > minLen) {
                return;
            }
            bool * ssik = &selectedSets[i][key];
            QList<int> * aik = &accepted[i][key];
            if (!*ssik) {
                *ssik = true;
                for (int j = 0; j < (*aik).length(); ++j) {
                    if (selectedOnes[(*aik)[j]] == 0) ++ones;
                    selectedOnes[(*aik)[j]] += 1;
                }
                QString hed = headMap[cnt][i - 1];
                curDNF << getTerm(hed, key);
                findMDNF(curLen + key.length(), curDNF, idx + 1, ones);
                curDNF.removeLast();
                *ssik = false;
                for (int j = 0; j < (*aik).length(); ++j) {
                    selectedOnes[(*aik)[j]] -= 1;
                    if (selectedOnes[(*aik)[j]] == 0){
                        --ones;
                    }
                }
            }
        }
    }
}

void MainWindow::certainMDNF() {
    QTextBrowser * lbl = ui->lblAns;
    QLabel * lblGif = ui->lblGif;
    QStringList garbageList;
    findMDNF(0, garbageList, 0, 0);
    QString s1, s2;
    s1.setNum(minLen);
    s2.setNum(answers[minLen].length());
    if (minLen == 0) {
        s2.setNum(0);
    }
    QString strr;
    for (int i = 0; i < answers[minLen].length(); ++i) {
        strr += answers[minLen][i].join(" + ") + "\n";
    }
    lbl->setText(strr);
    lblGif->clear();
}

void MainWindow::on_btnApply_clicked() {
    QString tmpcntS = ui->funcNumberInput2_2->text();
    bool ok1, ok2;
    int tmpcnt = tmpcntS.toInt(&ok1);
    QString stringInput;
    stringInput = QString::number(ui->funcNumberInput2->text().toInt(&ok2), 2);
    if (!QRegularExpression("[0-9]*").match(stringInput).hasMatch()) {
        QMessageBox::about(this, "Error!", "Incorrect input");
        return;
    }
    for (int i = 0; pow(2, tmpcnt)-stringInput.length() > 0; ++i) {
        stringInput = "0" + stringInput;
    }
    if(stringInput == currentFunction) {
        return;
    }
    if (stringInput.length() > pow(2, tmpcnt)) {
        QMessageBox::about(this, "Error!", "Incorrect input");
        return;
    }
    updateCount(tmpcnt);
    setOriginalState();
    currentFunction = stringInput;
    buildTable();
}

void MainWindow::on_pushButton_clicked() {
    if(currAction == 0) {
        for (int i =0 ; i < cntP2; ++i) {
            QTableWidgetItem * itm = mainTable->item(i,0);
            if (itm->text() == "0") {
                for (int j = 1; j < cntP2; ++j) {
                    QCoreApplication::processEvents();
                    QTableWidgetItem * tmpItm = mainTable->item(i,j);
                    tmpItm->setBackground(QColor(200, 0, 0, 100));
                    if (!isBanned(j, tmpItm->text())) {
                        banned[j] << tmpItm->text();
                    }
                }
            }
        }
    }
    else if(currAction == 1) {
        for (int i =0 ; i < cntP2; ++i) {
            QTableWidgetItem * itm = mainTable->item(i,0);
            if (itm->text() == "1") {
                ++needOnes;
                selectedOnes[i+1] = false;
                for (int j = 1; j < cntP2; ++j) {
                    QCoreApplication::processEvents();
                    QTableWidgetItem * tmpItm = mainTable->item(i, j);
                    if(isBanned(j, tmpItm->text())) {
                        tmpItm->setBackground(QColor(0, 0, 150, 127));
                    } else {
                        tmpItm->setBackground(QColor(0, 150, 0, 127));
                        accepted[j][tmpItm->text()] << i + 1;
                    }
                }
            }
        }
    }
    else if(currAction == 2) {
        certainMDNF();
    }
    if(currAction < 3 && currAction != -1)
        ++currAction;
}

void MainWindow::on_pushButton_2_clicked() {
    if(currAction == 3) {
        ui->lblAns->clear();
    }
    else if(currAction == 2) {
        for (int i =0 ; i < cntP2; ++i) {
            QTableWidgetItem * itm = mainTable->item(i,0);
            if (itm->text() == "1") {
                ++needOnes;
                selectedOnes[i+1] = false;
                for (int j = 1; j < cntP2; ++j) {
                    QCoreApplication::processEvents();
                    QTableWidgetItem * tmpItm = mainTable->item(i, j);
                    if(isBanned(j,tmpItm->text())) {
                        tmpItm->setBackground(QBrush());
                    } else {
                        tmpItm->setBackground(QBrush());
                        accepted[j][tmpItm->text()] << i + 1;
                    }
                }
            }
        }
    }
    else if(currAction == 1) {
        for (int i =0 ; i < cntP2; ++i) {
            QTableWidgetItem * itm = mainTable->item(i,0);
            if (itm->text() == "0") {
                for (int j = 1; j < cntP2; ++j) {
                    QCoreApplication::processEvents();
                    QTableWidgetItem * tmpItm = mainTable->item(i, j);
                    tmpItm->setBackground(QBrush());
                    if (!isBanned(j, tmpItm->text())) {
                        banned[j] << tmpItm->text();
                    }
                }
            }
        }
    }
    if(currAction > 0 && currAction != -1)
        --currAction;
}

QStringList MainWindow::generateSubsets(int n) {
    QStringList result;
    QString letters = "abcdef";
    int totalSubsets = pow(2, n);
    for (int i = 1; i < totalSubsets; ++i) {
        QString subset;
        for (int j = 0; j < n; ++j) {
            if (i & (1 << j)) {
                subset += letters[j];
            }
        }
        result << subset;
    }
    std::sort(result.begin(), result.end(), [](const QString &a, const QString &b) {
        return a.length() == b.length() ? a < b : a.length() < b.length();
    });
    return result;
}

void MainWindow::fillHeadMap(QMap<int, QStringList> &headMap, int maxN) {
    for (int i = 1; i <= maxN; ++i) {
        headMap[i] = generateSubsets(i);
    }
}
