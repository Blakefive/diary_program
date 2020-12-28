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
        cout << "잘못 입력했습니다. 다시 한 번 더 확인하고 입력해주세요." << endl;
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
        cout << "수정할 데이터의 번호를 말씀해주세요.(1부터 시작 " << datasize() << " 까지)" << endl;
        cin >> modifyposition;
        if (cin.fail() == true) {
            inputerror(modifyposition);
        }
        cout << modifyposition << "번쨰의 일기: " << data[modifyposition - 1] << endl;
        cout << "수정 내용: ";
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
    string filePath = "!!main로그인\\login.txt";
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
            cout << "닉네임(공백 x): ";
            cin >> name;
            cout << "비밀번호(공백 x): ";
            passnumber = inputsecret();
            if (vectorcheck(namevector, passnumbervector, name, passnumber) == 1) {
                return name;
            }
            else {
                cout << "닉네임이나 비밀번호가 틀렸습니다." << endl;
                return "";
            }
        }
        else {
            cout << "회원 가입을 해주세요." << endl;
            return "";
        }
    }
};

class newlogin {
private:
    string name;
    string pass;
    string passcheck;
    string filePath = "!!main로그인\\login.txt";
    encryption encryptionmain;
    string line;
    vector<string>linevector;
    login loginmain;
public:
    void newloginstart() {
        cout << "닉네임을 적어주세요.: ";
        cin >> name;
        cout << "비밀번호를 적어주세요.: ";
        pass = loginmain.inputsecret();
        while (1) {
            cout << "비밀번호를 다시 한번 작성해주세요.(비밀번호 다시 작성하기: r): ";
            passcheck = loginmain.inputsecret();
            if (passcheck == "r") {
                cout << "비밀번호를 적어주세요.: ";
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
        cout << "회원 가입이 되었습니다." << endl;
    }
};

class everylogin {
private:
    string filePath = "!!main로그인\\login.txt";
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
        SetFileAttributesA("!!main로그인\\login.txt", FILE_ATTRIBUTE_HIDDEN);
    }
    void loginnothidden() {
        SetFileAttributesA("!!main로그인\\login.txt", FILE_ATTRIBUTE_NORMAL);
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
            cout << "닉네임 리스트: ";
            for (int i = 0; i < data.size(); ++i) {
                cout << data[i] << " ";
            }
            cout << endl;
        }
        else {
            cout << "회원 가입을 해주세요." << endl;
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
            cout << "닉네임 순서: ";
            for (int i = 0; i < data.size(); ++i) {
                cout << data[i] << " " << i + 1 << " ";
            }
            cout << endl;
            cout << "탈퇴할 계정의 순서를 말하세요.(1부터 " << data.size() << "까지): ";
            cin >> delloginposition;
            delloginposition = inputerror(delloginposition);
            while (1) {
                cout << delloginposition << "번째 계정의 비밀번호를 작성하세요.: ";
                pass = loginmain.inputsecret();
                if (datapass[delloginposition - 1] == pass) {
                    string h = "rd /s /q .\\" + data[delloginposition - 1];
                    system(h.c_str());
                    data.erase(data.begin() + delloginposition - 1);
                    datapass.erase(datapass.begin() + delloginposition - 1);
                    break;
                }
                cout << "비밀번호가 틀렸습니다(다시 시도: another key, 취소: c): " << endl;
                cin >> inputdata;
                if (inputdata == 'c') {
                    cout << "계정 탈퇴를 멈춥니다" << endl;
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
            cout << "성공적으로 " << delloginposition << "번째 계정을 탈퇴했습니다." << endl;
        }
        else {
            cout << "회원 가입을 해주세요." << endl;
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
    SetFileAttributesA("!!main로그인", FILE_ATTRIBUTE_HIDDEN);
    while (1) {
        char N;
        cout << "로그인: l, 회원가입: n, 계정 닉네임 리스트:e, 계정 탈퇴: d, 멈추기: s: ";
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
            cout << "잘못 입력하셨습니다. 다시 한번 더 확인하고 입력해주세요." << endl;
            continue;
        }
    }
    dkdk.loginhidden();
    if (stopcheck != 1) {
        secret main;
        main.firststart(name);
        while (1) {
            char N;
            cout << "추가: a, 데이터 가져오기: g, 데이터 날짜 리스트 가져오기: f, 지우기: d, 수정: m, 로그아웃: l, 멈추기: s: ";
            cin >> N;
            if (N == 'a') {
                string data;
                cout << "일기 내용: ";
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
                    cout << "데이터의 번호를 말씀해주세요.(1부터 시작 " << main.datasize() << " 까지)" << endl;
                    cin >> number;
                    number = inputerror(number);
                    vector<string> data = main.output(number);
                    cout << "날짜: " << data[0] << " 일기 내용: " << data[1] << endl;
                }
                else {
                    cout << "입력된 데이터가 없습니다." << endl;
                }
            }
            if (N == 'f') {
                if (main.datasize() != 0) {
                    cout << "날짜 리스트: ";
                    vector<string> localtimedatalist = main.localtimedata();
                    for (int i = 0; i < localtimedatalist.size(); ++i) {
                        cout << localtimedatalist[i] << " ";
                    }
                    cout << endl;
                }
                else {
                    cout << "입력된 데이터가 없습니다." << endl;
                }
            }
            if (N == 'd') {
                if (main.datasize() != 0) {
                    int number;
                    main.dataprint();
                    cout << "지울 데이터의 번호를 말씀해주세요.(1부터 시작 " << main.datasize() << " 까지)" << endl;
                    cin >> number;
                    number = inputerror(number);
                    main.del(number, name);
                }
                else {
                    cout << "입력된 데이터가 없습니다." << endl;
                }
            }
            if (N == 'm') {
                if (main.datasize() != 0) {
                    main.modify();
                }
                else {
                    cout << "입력된 데이터가 없습니다." << endl;
                }
            }
            if (N == 'l') {
                goto loginagain;
            }
            if (N == 's') {
                break;
            }
            if (cin.fail() == true) {
                cout << "잘못 입력하셨습니다. 다시 한번 더 확인하고 입력해주세요." << endl;
                continue;
            }
        }
    }
    dkdk.loginhidden();
    return 0;
}