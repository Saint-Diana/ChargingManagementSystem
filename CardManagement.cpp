//
// Created by 沈慧昌 on 2023/2/24.
//

#include "CardManagement.h"


string time_t2string(const time_t time_t_time)
{
    char szTime[100] = {'\0'};

    tm *pTm = new tm;
    localtime_s(pTm, &time_t_time);
    //pTm = localtime(&time_t_time);
    pTm->tm_year += 1900;
    pTm->tm_mon +=1;

    sprintf_s(szTime, "%04d-%02d-%02d %02d:%02d:%02d",
              pTm->tm_year,
              pTm->tm_mon,
              pTm->tm_mday,
              pTm->tm_hour,
              pTm->tm_min,
              pTm->tm_sec);

    string strTime = szTime;

    delete pTm;
    pTm = NULL;

    return strTime;
}

time_t string2time_t(const string string_time)
{
    tm tm1;
    memset(&tm1, 0, sizeof(tm1));
    time_t time1;

    sscanf_s(string_time.c_str(), "%d/%d/%d/ %d:%d:%d",
             &(tm1.tm_year),
             &(tm1.tm_mon),
             &(tm1.tm_mday),
             &(tm1.tm_hour),
             &(tm1.tm_min),
             &(tm1.tm_sec));

    tm1.tm_year -= 1900;
    tm1.tm_mon -= 1;

    time1 = mktime(&tm1);

    return time1;

}


CardManagement::CardManagement() {
    admin = Admin();
    readCards();
    readAdmin();
    readMachines();
    readRecords();
    readPrice();
    //machines.txt文件中没有数据时发挥作用
    for(int i = 0;i < MAX_MACHINE_NUM;++i)
        machines[i].id = i + 1;
}

CardManagement::~CardManagement() {
    saveCards();
    saveMachines();
    saveRecords();
    savePrice();
}


/**
 * 添加卡
 * @param cardId 卡号
 * @param password 密码
 * @param money 开卡金额
 */
void CardManagement::addCard(string cardId, string password, double money) {
    if(cardNum >= MAX_CARD_NUM){
        printf("卡已达到数量上限！");
        return;
    }
    if(isExist(cardId)){
        printf("该卡已存在，请勿重复添加！\n");
        return;
    }
    Card card(cardId, password, money);
    cards.add(card);
    cardNum++;
    printf("添加卡成功\n");
}

/**
 * 查找系统中该卡号是否已存在
 * @param cardId 卡号
 * @return 是否存在
 */
bool CardManagement::isExist(string cardId) {
    for(int i = 0; i < cardNum; ++i){
        if(cardId == cards[i].cardId){
            return true;
        }
    }
    return false;
}

/**
 * 精确查询
 * @param cardId 卡号
 * @return
 */
bool CardManagement::accurateQuery(string cardId) {
    for(int i = 0; i < cardNum; ++i){
        if(cards[i].cardId == cardId){
            printf("卡号\t\t卡状态\t\t余额\t\t累计金额\t\t使用次数\t\t\t上次使用时间\t\t开卡时间\n");
            cout << cards[i].cardId << "\t\t";
            if(cards[i].state == 0)
                cout << "注销\t\t";
            else if(cards[i].state == 1)
                cout << "未上机\t\t";
            else
                cout << "上机\t\t";
            cout << cards[i].money << "\t\t";
            cout << cards[i].historyMoney << "\t\t\t\t";
            cout << cards[i].frequency << "\t\t";
            //输出时间
            string useTime = time_t2string(cards[i].time);
            cout << useTime << "\t\t";
            string openTime = time_t2string(cards[i].startTime);
            cout << openTime << endl;
            return true;
        }
    }
    printf("该卡不存在！\n");
    return false;
}

/**
 * 模糊查询
 * @param cardId 卡号
 * @return
 */
bool CardManagement::fuzzyQuery(string cardId) {
    bool flag = false;
    for(int i = 0; i < cardNum; ++i){
        if(cards[i].cardId.find(cardId) != string::npos){
            flag = true;
            printf("卡号\t\t卡状态\t\t余额\t\t累计金额\t\t使用次数\t\t\t上次使用时间\t\t开卡时间\n");
            cout << cards[i].cardId << "\t\t";
            if(cards[i].state == 0)
                cout << "注销\t\t";
            else if(cards[i].state == 1)
                cout << "未上机\t\t";
            else
                cout << "上机\t\t";
            cout << cards[i].money << "\t\t";
            cout << cards[i].historyMoney << "\t\t\t\t";
            cout << cards[i].frequency << "\t\t";
            //输出时间
            string useTime = time_t2string(cards[i].time);
            string openTime = time_t2string(cards[i].startTime);
            cout << useTime << "\t\t" << openTime << endl;
        }
    }
    if(!flag)
        printf("卡不存在！\n");
    return flag;
}


/**
 * 上机
 * @return 机器编号。若此时没有空闲机器，则返回-1。
 */
void CardManagement::login(string cardId, const string& password) {
    //首先判断是否存在该卡
    if(!isExist(cardId)) {
        printf("该卡不存在！\n");
        return;
    }
    for(int i = 0; i < cardNum; ++i){
        if(cards[i].cardId == cardId){
            //然后判断密码是否正确
            if (cards[i].password != password) {
                printf("密码错误！\n");
                return;
            }
            //接下来判断该卡是否注销
            if(cards[i].state == 0) {
                printf("该卡已注销！\n");
                return;
            }
            //接下来判断该卡是否可用
            if(cards[i].state == 2) {
                printf("该卡已上机！\n");
                return;
            }
            //如果该卡余额连一秒也不够，那么就不允许上机
            if(cards[i].money <= price_per_second){
                printf("该卡余额不足，请及时充值！\n");
                return;
            }
            //接下来找是否存在可用机器
            if(!isExistFreeMachine()){
                printf("机器全部被占用，请耐心等待！\n");
                return;
            }
            //如果存在可用机器，那就可以让该卡上机
            for(int j = 0;j < MAX_MACHINE_NUM;++j){
                if(machines[j].isFree){
                    //修改卡字段表示卡已上机
                    cards[i].state = 2;
                    cards[i].frequency++;
                    auto now = std::chrono::system_clock::now();
                    cards[i].time = std::chrono::system_clock::to_time_t(now);
                    //修改机器字段
                    machines[j].isFree = false;
                    machines[j].cardId = cards[i].cardId;
                    machines[j].time = std::chrono::system_clock::to_time_t(now);
                    //打印信息
                    printf("机器编号\t\t卡号\t\t余额\t\t上机时间\n");
                    cout << machines[j].id << "\t\t";
                    cout << machines[j].cardId << "\t\t";
                    cout << cards[i].money << "\t\t";
                    string useTime = time_t2string(machines[j].time);
                    cout << useTime << endl;
                    return;
                }
            }
        }
    }

}

void CardManagement::logout(string cardId, const string &password) {
    //首先判断是否存在该卡
    if(!isExist(cardId)) {
        printf("该卡不存在！\n");
        return;
    }
    for(int i = 0; i < cardNum; ++i){
        if (cards[i].cardId == cardId) {
            //然后判断密码是否正确
            if (cards[i].password != password) {
                printf("密码错误！\n");
                return;
            }
            //接下来判断该卡是否注销
            if(cards[i].state == 0) {
                printf("该卡已注销！\n");
                return;
            }
            //接下来判断该卡是否已上机
            if(cards[i].state == 1) {
                printf("该卡未上机！\n");
                return;
            }
            //接下来就将该卡下机，并且修改机器信息
            printf("机器编号\t\t卡号\t\t余额\t\t上机时间\t\t下机时间\n");
            for(int j = 0;j < MAX_MACHINE_NUM;++j){
                if(machines[j].cardId == cards[i].cardId && !machines[j].isFree) {
                    machines[j].isFree = true;
                    cout << machines[j].id << "\t\t";
                }
            }
            cards[i].state = 1;
            auto now = std::chrono::system_clock::now();
            time_t current  = std::chrono::system_clock::to_time_t(now);
            long seconds = current - cards[i].time;
            cards[i].money -= price_per_second * seconds;
            //打印信息
            cout << cards[i].cardId << "\t\t";
            cout << cards[i].money << "\t\t";
            //输出上机时间
            string loginTime = time_t2string(cards[i].time);
            cout << loginTime << "\t";
            //输出下机时间
            auto logoutTime = time_t2string(current);
            cout << logoutTime << endl;
            Record record(cards[i].cardId, cards[i].time, current, price_per_second * seconds);
            records.add(record);
            recordNum++;
            return;
        }
    }
}

bool CardManagement::isExistFreeMachine() {
    for(int i = 0;i < MAX_MACHINE_NUM;++i){
        if (machines[i].isFree){
            return true;
        }
    }
    return false;
}


void CardManagement::charge(string cardId, const string& password, double money){
    //首先判断是否存在该卡
    if(!isExist(cardId)) {
        printf("该卡不存在！\n");
        return;
    }
    for(int i = 0; i < cardNum; ++i) {
        if (cards[i].cardId == cardId) {
            //然后判断密码是否正确
            if (cards[i].password != password) {
                printf("密码错误！\n");
                return;
            }
            //接下来判断该卡是否注销
            if (cards[i].state == 0) {
                printf("该卡已注销！\n");
                return;
            }
            //若该卡密码正确且未注销，则进行充值
            cards[i].money += money;
            cards[i].historyMoney += money;
            printf("充值成功！\n");
            printf("----充值信息如下----\n");
            printf("卡号\t\t充值金额\t\t余额\n");
            cout << cards[i].cardId << "\t\t" << money << "\t\t" << cards[i].money << endl;
            return;
        }
    }
}



void CardManagement::refund(string cardId, const string& password, double money) {
    //首先判断是否存在该卡
    if(!isExist(cardId)) {
        printf("该卡不存在！\n");
        return;
    }
    for(int i = 0; i < cardNum; ++i) {
        if (cards[i].cardId == cardId) {
            //然后判断密码是否正确
            if (cards[i].password != password) {
                printf("密码错误！\n");
                return;
            }
            //接下来判断该卡是否注销
            if (cards[i].state == 0) {
                printf("该卡已注销！\n");
                return;
            }
            //若该卡密码正确且未注销，则进行退费
            //首先判断退费金额是否大于余额
            if(money > cards[i].money){
                printf("退费金额大于余额，退费失败！\n");
                return;
            }
            //若小于，则进行退费
            cards[i].money -= money;
            printf("退费成功！\n");
            printf("----退费信息如下----\n");
            printf("卡号\t\t退费金额\t\t余额\n");
            cout << cards[i].cardId << "\t\t" << money << "\t\t" << cards[i].money << endl;
            return;
        }
    }
}


void CardManagement::deleteCard(string cardId, const string& password){
    //首先判断是否存在该卡
    if(!isExist(cardId)) {
        printf("该卡不存在！\n");
        return;
    }
    for(int i = 0; i < cardNum; ++i) {
        if (cards[i].cardId == cardId) {
            //然后判断密码是否正确
            if (cards[i].password != password) {
                printf("密码错误！\n");
                return;
            }
            //接下来判断该卡是否注销
            if (cards[i].state == 0) {
                printf("该卡已注销！\n");
                return;
            }
            //若密码正确且该卡未注销，则进行注销操作
            printf("注销成功！\n");
            printf("----注销信息如下----\n");
            printf("卡号\t\t退费金额\n");
            cout << cards[i].cardId << "\t\t" << cards[i].money << endl;
            cards[i].state = 0;
            cards[i].money = 0;
            return;
        }
    }
}


void CardManagement::recoverCard(string adminName, string adminPassword, string cardId, const string& password){
    //首先需要验证管理员身份
    if(!this->admin.isLegal(adminName, adminPassword)){
        printf("管理员账号或密码错误！\n");
        return;
    }
    //只有验证成功，才能使用管理员功能
    //首先判断是否存在该卡
    if(!isExist(cardId)) {
        printf("该卡不存在！\n");
        return;
    }
    for(int i = 0; i < cardNum; ++i) {
        if (cards[i].cardId == cardId) {
            //然后判断密码是否正确
            if (cards[i].password != password) {
                printf("卡密码错误！\n");
                return;
            }
            //接下来判断该卡是否注销
            if (cards[i].state == 1) {
                printf("该卡未注销！\n");
                return;
            }
            //若密码正确且该卡已注销，则进行恢复操作
            printf("恢复成功！\n");
            printf("----恢复信息如下----\n");
            printf("卡号\t\t余额\n");
            cout << cards[i].cardId << "\t\t" << cards[i].money << endl;
            cards[i].state = 1;
            return;
        }
    }
}

void CardManagement::printAllMachines(string username, string password) {
    if(!admin.isLegal(username, password)){
        printf("管理员账号或密码错误！\n");
        return;
    }
    printf("----所有机器信息如下----\n");
    printf("机器编号\t\t机器状态\n");
    for(int i = 0;i < MAX_MACHINE_NUM;++i){
        cout << machines[i].id << "\t\t";
        if(machines[i].isFree)
            cout << "空闲" << endl;
        else
            cout << "正在使用" << endl;
    }
}

void CardManagement::printAllCards(string username, string password){
    if(!admin.isLegal(username, password)){
        printf("管理员账号或密码错误！\n");
        return;
    }
    if(cardNum == 0){
        printf("暂时无卡！\n");
        return;
    }
    printf("----所有卡信息如下----\n");
    printf("卡号\t\t卡状态\t\t余额\t\t累计金额\t\t使用次数\t\t\t上次使用时间\t\t\t开卡时间\n");
    for(int i = 0; i < cardNum; ++i){
        cout << cards[i].cardId << "\t\t";
        if(cards[i].state == 0)
            cout << "注销\t\t";
        else
            cout << "正常使用\t\t";
        cout << cards[i].money << "\t\t";
        cout << cards[i].historyMoney << "\t\t\t\t";
        cout << cards[i].frequency << "\t\t";
        //输出时间
        string strTime = time_t2string(cards[i].time);
        cout << strTime << "\t\t";
        strTime = time_t2string(cards[i].startTime);
        cout << strTime << endl;
    }
}

void CardManagement::cardRecords(time_t beginTime, time_t endTime) {
    int cnt = 0;
    double sum = 0;
    cout << "开卡情况统计报表" << endl;
    cout << "卡号" << "\t\t" << "开卡时间" << "\t\t\t\t" << "累计充值" << endl;
    for(int i = 0; i < cardNum; ++i){
        time_t openTime = cards[i].startTime;
        if(openTime >= beginTime && openTime <= endTime){
            cout << cards[i].cardId << "\t\t";
            cout << time_t2string(cards[i].startTime) << "\t\t";
            cout << cards[i].historyMoney << endl;
            cnt++;
            sum += cards[i].historyMoney;
        }
    }
    cout << "合计：" << cnt << "张\t\t总营业额" << sum <<   "元" << endl;
}

void CardManagement::consumeRecords(time_t beginTime, time_t endTime) {
    double sum = 0;
    cout << "开卡情况统计报表" << endl;
    cout << "卡号" << "\t\t" << "上机时间" << "\t\t" << "下机时间" << "\t\t" << "消费金额" << endl;
    for(int i = 0;i < recordNum;++i){
        if(records[i].beginTime >= beginTime && records[i].beginTime <= endTime && records[i].endTime >= beginTime && records[i].endTime <= endTime){
            cout << records[i].cardId << "\t\t";
            cout << time_t2string(records[i].beginTime) << "\t\t";
            cout << time_t2string(records[i].endTime) << "\t\t";
            cout << records[i].money << endl;
            sum += records[i].money;
        }
    }
    cout << "合计：" << sum << "元" << endl;
}

void CardManagement::changePrice(string username, string password, double price) {
    if(!admin.isLegal(username, password)){
        printf("管理员账号或密码错误！\n");
        return;
    }
    price_per_second = price;
    cout << "修改成功！" << endl;
}

void CardManagement::saveCards() {
    // 以写入模式打开文件
    FILE *fp = fopen(CARDS_PATH, "w");
    if (fp == nullptr)
    {
        // 打开文件失败
        cerr << "打开文件cards.txt失败!" << endl;
        return;
    }
    // 使用 fputs() 写入字符串
    //首先写入卡的数量num
    fputs((to_string(cardNum) + "\n").c_str(), fp);
    //然后写入每一条卡记录的信息
    for(int i = 0; i < cardNum; ++i){
        string info = cards[i].cardId + " " + cards[i].password + " "
                + to_string(cards[i].money) + " " + to_string(cards[i].historyMoney) + " "
                + to_string(cards[i].state) + " " + to_string(cards[i].frequency) + " "
                + to_string(cards[i].time) + " " + to_string(cards[i].startTime) + "\n";
        fputs(info.c_str(), fp);
    }
    fclose(fp);
}

void CardManagement::readCards() {
    // 以读取模式打开文件
    ifstream file(CARDS_PATH);
    if(!file.is_open()){
        // 打开文件失败
        cerr << "打开文件cards.txt失败!" << endl;
        return;
    }
    string data;
    //首先读入卡的数量
    file >> data;
    cardNum = stoi(data);
    //然后读入卡的信息
    int cnt = 0;
    Card card;
    while (file >> data){
        switch (cnt % 8) {
            case 0:{
                card.cardId = data;
                break;
            }
            case 1:{
                card.password = data;
                break;
            }
            case 2:{
                card.money = stod(data);
                break;
            }
            case 3:{
                card.historyMoney = stod(data);
                break;
            }
            case 4:{
                card.state = stoi(data);
                break;
            }
            case 5:{
                card.frequency = stoi(data);
                break;
            }
            case 6:{
                card.time = stoi(data);
                break;
            }
            case 7:{
                card.startTime = stoi(data);
                cards.add(card);
                break;
            }
        }
        cnt++;
    }
    // 关闭文件
    file.close();
}

void CardManagement::readAdmin() {
    // 以读取模式打开文件
    ifstream file(ADMIN_PATH);
    if(!file.is_open()){
        // 打开文件失败
        cerr << "打开文件admin.txt失败!" << endl;
        return;
    }
    string data;
    file >> data;
    admin.username = data;
    file >> data;
    admin.password = data;
    // 关闭文件
    file.close();
}

void CardManagement::saveMachines() {
    // 以写入模式打开文件
    FILE *fp = fopen(MACHINES_PATH, "w");
    if (fp == nullptr)
    {
        // 打开文件失败
        cerr << "打开文件machines.txt失败!" << endl;
        return;
    }
    // 使用 fputs() 写入字符串
    //写入每一台机器的信息
    for(int i = 0;i < MAX_MACHINE_NUM;++i){
        string info = to_string(machines[i].id) + " " + to_string(machines[i].isFree) + " "
                + machines[i].cardId + " " + to_string(machines[i].time) + "\n";
        fputs(info.c_str(), fp);
    }
    fclose(fp);
}

void CardManagement::readMachines() {
    // 以读取模式打开文件
    ifstream file(MACHINES_PATH);
    if(!file.is_open()){
        // 打开文件失败
        cerr << "打开文件machines.txt失败!" << endl;
        return;
    }
    string data;
    //读入机器的信息
    int cnt = 0;
    Machine machine;
    while (file >> data){
        switch (cnt % 4) {
            case 0:{
                machine.id = stoi(data);
                break;
            }
            case 1:{
                machine.isFree = stoi(data);
                break;
            }
            case 2:{
                machine.cardId = data;
                break;
            }
            case 3:{
                machine.time = stoi(data);
                machines.add(machine);
                break;
            }
        }
        cnt++;
    }
    // 关闭文件
    file.close();
}


void CardManagement::saveRecords() {
    // 以写入模式打开文件
    FILE *fp = fopen(RECORDS_PATH, "w");
    if (fp == nullptr)
    {
        // 打开文件失败
        cerr << "打开文件records.txt失败!" << endl;
        return;
    }
    // 使用 fputs() 写入字符串
    //首先写入消费记录的数量num
    fputs((to_string(recordNum) + "\n").c_str(), fp);
    //然后写入每一条卡记录的信息
    for(int i = 0; i < recordNum; ++i){
        string info = records[i].cardId + " " + to_string(records[i].beginTime) + " "
                      + to_string(records[i].endTime) + " " + to_string(records[i].money) + "\n";
        fputs(info.c_str(), fp);
    }
    fclose(fp);
}

void CardManagement::readRecords() {
    // 以读取模式打开文件
    ifstream file(RECORDS_PATH);
    if(!file.is_open()){
        // 打开文件失败
        cerr << "打开文件records.txt失败!" << endl;
        return;
    }
    string data;
    //首先读入卡的数量
    file >> data;
    recordNum = stoi(data);
    //然后读入卡的信息
    int cnt = 0;
    Record record;
    while (file >> data){
        switch (cnt % 4) {
            case 0:{
                record.cardId = data;
                break;
            }
            case 1:{
                record.beginTime = stoi(data);
                break;
            }
            case 2:{
                record.endTime = stoi(data);
                break;
            }
            case 3:{
                record.money = stod(data);
                records.add(record);
                break;
            }
        }
        cnt++;
    }
    // 关闭文件
    file.close();
}

void CardManagement::readPrice() {
    // 以读取模式打开文件
    ifstream file(PRICE_PATH);
    if(!file.is_open()){
        // 打开文件失败
        cerr << "打开文件price.txt失败!" << endl;
        return;
    }
    string data;
    //首先读入卡的数量
    file >> data;
    price_per_second = stod(data);
    // 关闭文件
    file.close();
}

void CardManagement::savePrice() {
// 以写入模式打开文件
    FILE *fp = fopen(PRICE_PATH, "w");
    if (fp == nullptr)
    {
        // 打开文件失败
        cerr << "打开文件price.txt失败!" << endl;
        return;
    }
    // 使用 fputs() 写入字符串
    fputs((to_string(price_per_second) + "\n").c_str(), fp);
    fclose(fp);
}



