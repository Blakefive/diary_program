#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include<fstream>
#include<Windows.h>
#include <direct.h>
#include <stdio.h>
#include <conio.h>

using namespace std;

class encryption {
private:
    int loop = 19;
    string delstring;
    int randomdata() {
        srand(time(0));
        int num = rand() % 100;
        if (to_string(num).length() < 2) {
            num *= 10;
        }
        if (num == 0) {
            while (true) {
                int num = rand() % 99;
                if (to_string(num).length() < 2) {
                    num *= 10;
                }
                if (num != 0) {
                    break;
                }
            }
        }
        return num;
    }
    string encryptionstart(string data) {
        loop = randomdata();
        delstring = data.substr(1, data.length() - 1) + data[0];
        for (int i = 0; i < loop; ++i) {
            delstring = delstring.substr(1, delstring.length() - 1) + delstring[0];
        }
        return delstring + to_string(loop);
    }

    string encryptionend(string data) {
        loop = stoi(data.substr(data.length() - 2, data.length() - 1));
        data = data.substr(0, data.length() - 2);
        delstring = data[data.length() - 1] + data.substr(0, data.length() - 1);
        for (int i = 0; i < loop; ++i) {
            delstring = delstring[delstring.length() - 1] + delstring.substr(0, delstring.length() - 1);
        }
        return delstring;
    }
public:
    string encryptionstartpublic(string data) {
        return encryptionstart(data);
    }
    string encryptionendpublic(string data) {
        return encryptionend(data);
    }
};
int inputerror(int a) {
    while (cin.fail() == true) {
        cin.clear();
        cin.ignore(100, '\n');
        cout << "�߸� �Է��߽��ϴ�. �ٽ� �� �� �� Ȯ���ϰ� �Է����ּ���." << endl;
        cin >> a;
    }
    return a;
}

string todaylocalday() {
    time_t curr_time;
    struct tm* curr_tm;
    curr_time = time(NULL);
    curr_tm = localtime(&curr_time);
    return to_string(curr_tm->tm_year + 1900) + "." + to_string(curr_tm->tm_mon + 1) + "." + to_string(curr_tm->tm_mday);
}

class secret {
private:
    vector<string> localday;
    vector<string> data;
    int loopcheck = 0;
    string filepath = "";
    string modifystring;
    int modifyposition;
public:
    void firststart(string name) {
        localday = {};
        data = {};
        ifstream openFile((name + "\\main.txt").data());
        if (openFile.is_open()) {
            string line;
            while (getline(openFile, line)) {
                localday.push_back(line);
            }
            openFile.close();
        }
        for (int i = 0; i < localday.size(); ++i) {
            ifstream openFile((name + "\\" + localday[i] + ".txt").data());
            if (openFile.is_open()) {
                string line;
                while (getline(openFile, line)) {
                    data.push_back(line);
                }
                openFile.close();
            }
        }
        loopcheck = localday.size();
    }
    void dataprint() {
        for (int i = 0; i < localday.size(); ++i) {
            cout << localday[i] << " ";
        }
        cout << endl;
    }
    void input(string name, string data_input = "None") {
        loopcheck++;
        string localtime_input = todaylocalday() + "(" + to_string(loopcheck) + ")";
        localday.push_back(localtime_input);
        data.push_back(data_input);
        filepath = name + "\\" + localtime_input + ".txt";
        ofstream writeFile(filepath.data());
        if (writeFile.is_open()) {
            writeFile << data[loopcheck - 1];
            writeFile.close();
        }
        ofstream writefile((name + "\\main.txt").data());
        if (writefile.is_open()) {
            for (int i = 0; i < localday.size(); ++i) {
                writefile << localday[i] + "\n";
            }
            writefile.close();
        }
    }
    vector<string> output(int N) {
        return vector<string> {localday[N - 1], data[N - 1]};
    }
    int datasize() {
        return data.size();
    }
    vector<string> localtimedata() {
        return localday;
    }
    void del(int N, string name) {
        string delstringcheck = name + "\\" + localday[N - 1] + ".txt";
        remove(delstringcheck.c_str());
        localday.erase(localday.begin() + N - 1);
        data.erase(data.begin() + N - 1);
        if (localday.size() >= N) {
            for (int i = (N - 1); i < localday.size(); ++i) {
                string delfile = localday[i];
                delfile.pop_back();
                delfile.pop_back();
                delfile = delfile + to_string(i + 1) + ")";
                rename((name + "\\" + localday[i] + ".txt").c_str(), (name + "\\" + delfile + ".txt").c_str());
                localday.erase(localday.begin() + i);
                localday.insert(localday.begin() + i, delfile);
            }
        }
        loopcheck--;
        ofstream writeFile((name + "\\main.txt").data());
        if (writeFile.is_open()) {
            for (int i = 0; i < localday.size(); ++i) {
                writeFile << localday[i] + "\n";
            }
            writeFile.close();
        }
    }
    void modify() {
        dataprint();
        cout << "������ �������� ��ȣ�� �������ּ���.(1���� ���� " << datasize() << " ����)" << endl;
        cin >> modifyposition;
        if (cin.fail() == true) {
            inputerror(modifyposition);
        }
        cout << modifyposition << "������ �ϱ�: " << data[modifyposition - 1] << endl;
        cout << "���� ����: ";
        while (true) {
            getline(cin, modifystring);
            if (modifystring != "") {
                break;
            }
        }
        data[modifyposition - 1] = modifystring;
    }
};

class login {
private:
    string filePath = "!!main�α���\\login.txt";
    vector<string> namevector;
    vector<string>passnumbervector;
    int count = 0;
    string line;
    string name;
    string passnumber;
    encryption encryptionmain;
    char ch;
    string mainstring;
public:
    string inputsecret() {
        ch = 'a';
        mainstring = "";
        for (int i = 0; ch != 13; ++i) {
            ch = _getch();
            mainstring += ch;
        }
        cout << endl;
        return mainstring.substr(0, mainstring.length() - 1);
    }
    int vectorcheck(vector<string> namevector, vector<string> passnumbervector, string name, string passnumer) {
        int checkname = -1;
        int checkpass = -1;
        for (int i = 0; i < namevector.size(); ++i) {
            if (namevector[i] == name) {
                checkname = 1;
            }
            if (passnumbervector[i] == passnumer) {
                checkpass = 1;
            }
        }
        if (checkname == 1 && checkpass == 1) {
            return 1;
        }
        else {
            return 0;
        }
    }
    string loginmain() {
        ifstream openFile(filePath.data());
        if (openFile.is_open()) {
            while (getline(openFile, line)) {
                line = encryptionmain.encryptionendpublic(line);
                if (count % 2 == 0) {
                    namevector.push_back(line);
                }
                if (count % 2 == 1) {
                    passnumbervector.push_back(line);
                }
                count++;
            }
            openFile.close();
        }
        if (namevector.size() != 0) {
            cout << "�г���(���� x): ";
            cin >> name;
            cout << "��й�ȣ(���� x): ";
            passnumber = inputsecret();
            if (vectorcheck(namevector, passnumbervector, name, passnumber) == 1) {
                return name;
            }
            else {
                cout << "�г����̳� ��й�ȣ�� Ʋ�Ƚ��ϴ�." << endl;
                return "";
            }
        }
        else {
            cout << "ȸ�� ������ ���ּ���." << endl;
            return "";
        }
    }
};

class newlogin {
private:
    string name;
    string pass;
    string passcheck;
    string filePath = "!!main�α���\\login.txt";
    encryption encryptionmain;
    string line;
    vector<string>linevector;
    login loginmain;
public:
    void newloginstart() {
        cout << "�г����� �����ּ���.: ";
        cin >> name;
        cout << "��й�ȣ�� �����ּ���.: ";
        pass = loginmain.inputsecret();
        while (1) {
            cout << "��й�ȣ�� �ٽ� �ѹ� �ۼ����ּ���.(��й�ȣ �ٽ� �ۼ��ϱ�: r): ";
            passcheck = loginmain.inputsecret();
            if (passcheck == "r") {
                cout << "��й�ȣ�� �����ּ���.: ";
                pass = loginmain.inputsecret();
            }
            else if (pass == passcheck) {
                break;
            }
        }
        _mkdir(name.c_str());
        SetFileAttributesA(name.c_str(), FILE_ATTRIBUTE_HIDDEN);
        ofstream writefile((name + "\\main.txt").data());
        if (writefile.is_open()) {
            writefile.close();
        }
        ifstream openFile(filePath.data());
        if (openFile.is_open()) {
            while (getline(openFile, line)) {
                linevector.push_back(line);
            }
            openFile.close();
        }
        ofstream writeFile(filePath.data());
        if (writeFile.is_open()) {
            name = encryptionmain.encryptionstartpublic(name);
            pass = encryptionmain.encryptionstartpublic(pass);
            for (int i = 0; i < linevector.size(); ++i) {
                writeFile << linevector[i] + "\n";
            }
            writeFile << name + "\n";
            writeFile << pass + "\n";
            writeFile.close();
        }
        cout << "ȸ�� ������ �Ǿ����ϴ�." << endl;
    }
};

class everylogin {
private:
    string filePath = "!!main�α���\\login.txt";
    vector<string> data;
    vector<string> datapass;
    string line;
    string pass;
    int check = 0;
    int delloginposition;
    encryption encryptionmain;
    char inputdata;
    login loginmain;
public:
    void loginhidden() {
        SetFileAttributesA("!!main�α���\\login.txt", FILE_ATTRIBUTE_HIDDEN);
    }
    void loginnothidden() {
        SetFileAttributesA("!!main�α���\\login.txt", FILE_ATTRIBUTE_NORMAL);
    }
    void checklgin() {
        ifstream openFile(filePath.data());
        if (openFile.is_open()) {
            while (getline(openFile, line)) {
                if (check % 2 == 0) {
                    line = encryptionmain.encryptionendpublic(line);
                    data.push_back(line);
                }
                check++;
            }
            openFile.close();
        }
        if (data.size() != 0) {
            cout << "�г��� ����Ʈ: ";
            for (int i = 0; i < data.size(); ++i) {
                cout << data[i] << " ";
            }
            cout << endl;
        }
        else {
            cout << "ȸ�� ������ ���ּ���." << endl;
        }
    }
    void dellogin() {
        ifstream openFile(filePath.data());
        if (openFile.is_open()) {
            while (getline(openFile, line)) {
                line = encryptionmain.encryptionendpublic(line);
                if (check % 2 == 0) {
                    data.push_back(line);
                }
                else if (check % 2 == 1) {
                    datapass.push_back(line);
                }
                check++;
            }
            openFile.close();
        }
        if (data.size() != 0) {
            cout << "�г��� ����: ";
            for (int i = 0; i < data.size(); ++i) {
                cout << data[i] << " " << i + 1 << " ";
            }
            cout << endl;
            cout << "Ż���� ������ ������ ���ϼ���.(1���� " << data.size() << "����): ";
            cin >> delloginposition;
            delloginposition = inputerror(delloginposition);
            while (1) {
                cout << delloginposition << "��° ������ ��й�ȣ�� �ۼ��ϼ���.: ";
                pass = loginmain.inputsecret();
                if (datapass[delloginposition - 1] == pass) {
                    string h = "rd /s /q .\\" + data[delloginposition - 1];
                    system(h.c_str());
                    data.erase(data.begin() + delloginposition - 1);
                    datapass.erase(datapass.begin() + delloginposition - 1);
                    break;
                }
                cout << "��й�ȣ�� Ʋ�Ƚ��ϴ�(�ٽ� �õ�: another key, ���: c): " << endl;
                cin >> inputdata;
                if (inputdata == 'c') {
                    cout << "���� Ż�� ����ϴ�" << endl;
                    break;
                }
            }
            ofstream writerFile(filePath.data());
            if (writerFile.is_open()) {
                for (int i = 0; i < data.size(); ++i) {
                    writerFile << encryptionmain.encryptionstartpublic(data[i]) + "\n";
                    writerFile << encryptionmain.encryptionstartpublic(datapass[i]) + "\n";
                }
            }
            writerFile.close();
            cout << "���������� " << delloginposition << "��° ������ Ż���߽��ϴ�." << endl;
        }
        else {
            cout << "ȸ�� ������ ���ּ���." << endl;
        }
    }
};

int main() {
    ios::sync_with_stdio();
    string name;
    int stopcheck = 0;
    everylogin dkdk;
    dkdk.loginhidden();
loginagain:
    SetFileAttributesA("!!main�α���", FILE_ATTRIBUTE_HIDDEN);
    while (1) {
        char N;
        cout << "�α���: l, ȸ������: n, ���� �г��� ����Ʈ:e, ���� Ż��: d, ���߱�: s: ";
        cin >> N;
        if (N == 'l') {
            dkdk.loginnothidden();
            login loginmain;
            string logincheckname = loginmain.loginmain();
            if (logincheckname == "") {
                continue;
            }
            else {
                name = logincheckname;
                break;
            }
            dkdk.loginhidden();
        }
        if (N == 'n') {
            dkdk.loginnothidden();
            newlogin newloginmain;
            newloginmain.newloginstart();
            dkdk.loginhidden();
        }
        if (N == 'e') {
            dkdk.loginnothidden();
            everylogin everyloginmain;
            everyloginmain.checklgin();
            dkdk.loginhidden();
        }
        if (N == 'd') {
            dkdk.loginnothidden();
            everylogin evertloginmain;
            evertloginmain.dellogin();
            dkdk.loginhidden();
        }
        if (N == 's') {
            dkdk.loginhidden();
            stopcheck = 1;
            break;
        }
        if (cin.fail() == true) {
            cout << "�߸� �Է��ϼ̽��ϴ�. �ٽ� �ѹ� �� Ȯ���ϰ� �Է����ּ���." << endl;
            continue;
        }
    }
    dkdk.loginhidden();
    if (stopcheck != 1) {
        secret main;
        main.firststart(name);
        while (1) {
            char N;
            cout << "�߰�: a, ������ ��������: g, ������ ��¥ ����Ʈ ��������: f, �����: d, ����: m, �α׾ƿ�: l, ���߱�: s: ";
            cin >> N;
            if (N == 'a') {
                string data;
                cout << "�ϱ� ����: ";
                while (1) {
                    getline(cin, data);
                    if (data.length() > 0) {
                        break;
                    }
                }
                main.input(name, data);
            }
            if (N == 'g') {
                if (main.datasize() != 0) {
                    int number;
                    cout << "�������� ��ȣ�� �������ּ���.(1���� ���� " << main.datasize() << " ����)" << endl;
                    cin >> number;
                    number = inputerror(number);
                    vector<string> data = main.output(number);
                    cout << "��¥: " << data[0] << " �ϱ� ����: " << data[1] << endl;
                }
                else {
                    cout << "�Էµ� �����Ͱ� �����ϴ�." << endl;
                }
            }
            if (N == 'f') {
                if (main.datasize() != 0) {
                    cout << "��¥ ����Ʈ: ";
                    vector<string> localtimedatalist = main.localtimedata();
                    for (int i = 0; i < localtimedatalist.size(); ++i) {
                        cout << localtimedatalist[i] << " ";
                    }
                    cout << endl;
                }
                else {
                    cout << "�Էµ� �����Ͱ� �����ϴ�." << endl;
                }
            }
            if (N == 'd') {
                if (main.datasize() != 0) {
                    int number;
                    main.dataprint();
                    cout << "���� �������� ��ȣ�� �������ּ���.(1���� ���� " << main.datasize() << " ����)" << endl;
                    cin >> number;
                    number = inputerror(number);
                    main.del(number, name);
                }
                else {
                    cout << "�Էµ� �����Ͱ� �����ϴ�." << endl;
                }
            }
            if (N == 'm') {
                if (main.datasize() != 0) {
                    main.modify();
                }
                else {
                    cout << "�Էµ� �����Ͱ� �����ϴ�." << endl;
                }
            }
            if (N == 'l') {
                goto loginagain;
            }
            if (N == 's') {
                break;
            }
            if (cin.fail() == true) {
                cout << "�߸� �Է��ϼ̽��ϴ�. �ٽ� �ѹ� �� Ȯ���ϰ� �Է����ּ���." << endl;
                continue;
            }
        }
    }
    dkdk.loginhidden();
    return 0;
}