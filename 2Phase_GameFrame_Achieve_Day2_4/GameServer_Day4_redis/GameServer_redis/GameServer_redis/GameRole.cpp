#include "GameRole.h"
#include "GameProtocol.h"
#include "GameChannel.h"
#include "GameMsg.h"
#include "RandomName.h"
#include <iostream>
#include <string.h>
#include <random>
#include "TimerChannel.h"
#include <fstream>
#include <hiredis/hiredis.h>

class Exit_Game : public TimerProc
{
public:
    // 通过 TimerProc 继承
    void Proc() override
    {
        ZinxKernel::Zinx_Exit();
    }
    int GetTimerSec() override
    {
        return 7;
    }
} exit_Game;

static AOIWorld world(0, 400, 0, 400, 20, 20);
static std::default_random_engine random_engine(time(NULL));
RandomName randomName;

bool GameRole::Init()
{
    auto getnum_players = ZinxKernel::Zinx_GetAllRole();
    if (getnum_players.size() == 0)
    {
        TimerMng::GetInstance()->Del_Task(&exit_Game);
    }

    m_pid = m_protocol->m_channel->GetFd();
    // m_Name = std::string("Tom") + std::to_string(m_pid);
    /* 从随机姓名池任意取一个 */
	m_Name = randomName.GetName();

    /* 将姓名存入/tmp中方便查找 */
    std::ofstream ofs_name("/tmp/record_name", std::ios::app);
    /* 将名字存入redis缓存 */
    /*-------------------------------------------------------------*/
    redisContext* c = redisConnect("127.0.0.1", 6379);

    void* reply = redisCommand(c, "LPUSH game_name \"%s\"", m_Name.c_str());
    freeReplyObject(reply);

    redisFree(c);
    /*-------------------------------------------------------------*/

    ofs_name << m_Name << std::endl;

    
    // x = 100 + 3 * m_pid;
    // z = 100 + 3 * m_pid;
    /* 随机位置 */
    x = 100 + random_engine() % 50;
    z = 100 + random_engine() % 50;

    world.Add_Player(this);

    /* 向客户端发送上线消息 */
    // 但是这有什么意义呢???
    ZinxKernel::Zinx_SendOut(*CreateLoginIDName(), *m_protocol);
    /* 向客户端发送周围玩家信息 */
    ZinxKernel::Zinx_SendOut(*CreateSrdPlayerPosition(), *m_protocol);
    /* 向周围玩家发送自己位置信息 */
    auto player_list = world.GetSrdPlayerPosition(this);
    for (auto single : player_list)
    {
        auto player = dynamic_cast<GameRole*>(single);
        ZinxKernel::Zinx_SendOut(*CreateSelfPosition(), *player->m_protocol);
    }

    return true;
}

UserData* GameRole::ProcMsg(UserData& _poUserData)
{
    GET_REF2DATA(MultiMsg, multiMsg, _poUserData);

    for (auto gameMsg : multiMsg.m_msg_list)
    {
        switch (gameMsg->enMsgType)
        {
        case GameMsg::MSG_TYPE_CHAT_CONTENT:
            std::cout << "发送了一个对话" << std::endl;
            ProcChatMsg(gameMsg->pMsg);
            break;
        case GameMsg::MSG_TYPE_NEW_POSITION:
            ProcMoveMsg(gameMsg->pMsg);
            break;
        default:
            break;
        }
    }

    return nullptr;
}

void GameRole::Fini()
{
    auto get = ZinxKernel::Zinx_GetAllRole();
    if (get.size() == 0)
    {
        TimerMng::GetInstance()->Add_Task(&exit_Game);
    }
    // ZinxKernel::Zinx_SendOut(*CreateLogoffIDName(), *m_protocol);
    auto player_list = world.GetSrdPlayerPosition(this);
    /* 每次删除掉一个玩家后要从world中拿出来 */
    world.Del_Player(this);
    for (auto single : player_list)
    {
        auto player = dynamic_cast<GameRole*>(single);
        ZinxKernel::Zinx_SendOut(*CreateLogoffIDName(), *player->m_protocol);
    }

    /* 释放姓名 */
    randomName.ReleaseName(m_Name);

    /* 将姓名从redis中释放 */
    /*-------------------------------------------------------------*/
    redisContext* c = redisConnect("127.0.0.1", 6379);

    void* reply = redisCommand(c, "LREM game_name 0 \"%s\"", m_Name.c_str());
    freeReplyObject(reply);

    redisFree(c);
    /*-------------------------------------------------------------*/

    std::ifstream ifs_name("/tmp/record_name");
    std::string temp;
    std::list<std::string> cur_name_list;
    while (std::getline(ifs_name, temp))
    {
        if (temp != m_Name)
        {
			cur_name_list.push_back(temp);
        }
    }

    std::ofstream ofs_name("/tmp/record_name");
    for (auto single : cur_name_list)
    {
        ofs_name << single;
    }

    ifs_name.close();
    ofs_name.close();
}

GameMsg* GameRole::CreateLoginIDName()
{
    pb::SyncPid* pMsg = new pb::SyncPid();
    pMsg->set_pid(m_pid);
    pMsg->set_username(m_Name);

    GameMsg* pRetMsg = new GameMsg(GameMsg::MSG_TYPE_LOGIN_ID_NAME, pMsg);
    return pRetMsg;
}

GameMsg* GameRole::CreateSrdPlayerPosition()
{
    std::list<Player*>player_list = world.GetSrdPlayerPosition(this);
    pb::SyncPlayers* pMsg = new pb::SyncPlayers();
    auto msg_players = pMsg->mutable_ps();

    for (auto single : player_list)
    {
        auto player = dynamic_cast<GameRole*>(single);
        auto msg_player = msg_players->Add();
        msg_player->set_pid(player->m_pid);
        msg_player->set_username(player->m_Name);
        
        auto msg_pos = msg_player->mutable_p();
        msg_pos->set_x(player->x);
        msg_pos->set_y(player->y);
        msg_pos->set_z(player->z);
        msg_pos->set_v(player->v);

    }
    
    GameMsg* pRetMsg = new GameMsg(GameMsg::MSG_TYPE_SRD_POSITION, pMsg);

    return pRetMsg;
}

GameMsg* GameRole::CreateSelfPosition()
{
    pb::BroadCast* pMsg = new pb::BroadCast();
    pMsg->set_pid(m_pid);
    pMsg->set_username(m_Name);
    pMsg->set_tp(2);
    auto msg_pos = pMsg->mutable_p();
    msg_pos->set_x(x);
    msg_pos->set_y(y);
    msg_pos->set_z(z);
    msg_pos->set_v(v);
    GameMsg* pRetMsg = new GameMsg(GameMsg::MSG_TYPE_BROADCAST, pMsg);
    return pRetMsg;
}

GameMsg* GameRole::CreateLogoffIDName()
{
    pb::SyncPid* pMsg = new pb::SyncPid();
    pMsg->set_pid(m_pid);
    pMsg->set_username(m_Name);
    GameMsg* pRetMsg = new GameMsg(GameMsg::MSG_TYPE_LOGOFF_ID_NAME, pMsg);
    return pRetMsg;
}

void GameRole::ViewAppear(GameRole* _pGameRole)
{
    ZinxKernel::Zinx_SendOut(*CreateSelfPosition(), *_pGameRole->m_protocol);
    ZinxKernel::Zinx_SendOut(*_pGameRole->CreateSelfPosition(), *m_protocol);
}

void GameRole::ViewLost(GameRole* _pGameRole)
{
    ZinxKernel::Zinx_SendOut(*CreateLogoffIDName(), *_pGameRole->m_protocol);
    ZinxKernel::Zinx_SendOut(*_pGameRole->CreateLogoffIDName(), *m_protocol);
}

void GameRole::ProcChatMsg(google::protobuf::Message* _pmsg)
{
    pb::Talk* msg_talk = dynamic_cast<pb::Talk*>(_pmsg);
    std::string content = msg_talk->content();
    std::cout << "取出了一个消息:" << content << std::endl;

    pb::BroadCast* pMsg = new pb::BroadCast();
    pMsg->set_pid(m_pid);
    pMsg->set_username(m_Name);
    pMsg->set_tp(1);
    pMsg->set_contecnt(content);


    auto player_list = world.GetSrdPlayerPosition(this);
    for (auto single : player_list)
    {
		GameMsg* pRetMsg = new GameMsg(GameMsg::MSG_TYPE_BROADCAST, pMsg);
        auto player = dynamic_cast<GameRole*>(single);
        ZinxKernel::Zinx_SendOut(*pRetMsg, *player->m_protocol);
    }
}

void GameRole::ProcMoveMsg(google::protobuf::Message* _pmsg)
{
    /* 视野切换 */
    auto s1 = world.GetSrdPlayerPosition(this);

    pb::Position* msg_pos = dynamic_cast<pb::Position*>(_pmsg);

    world.Del_Player(this);

    x = msg_pos->x();
    y = msg_pos->y();
    z = msg_pos->z();
    v = msg_pos->v();

    world.Add_Player(this);

    auto s2 = world.GetSrdPlayerPosition(this);

    /* 视野出现 */
    for (auto single : s2)
    {
        auto player = dynamic_cast<GameRole*>(single);
        if (s1.end() == std::find(s1.begin(), s1.end(), player))
        {
            ViewAppear(player);
        }
    }

    /* 视野消失 */
    for (auto single : s1)
    {
        auto player = dynamic_cast<GameRole*>(single);
        if (s2.end() == std::find(s2.begin(), s2.end(), player))
        {
            ViewLost(player);
        }
    }

    pb::BroadCast* pMsg = new pb::BroadCast();
    pMsg->set_pid(m_pid);
    pMsg->set_username(m_Name);
    pMsg->set_tp(2);
    auto pos = pMsg->mutable_p();
    // pos->set_x((int)x);
    pos->set_x(x);
    pos->set_y(y);
    // pos->set_z((int)z);
    pos->set_z(z);
    pos->set_v(v);


    auto player_list = world.GetSrdPlayerPosition(this);
    for (auto single : player_list)
    {
		GameMsg* pRetMsg = new GameMsg(GameMsg::MSG_TYPE_BROADCAST, pMsg);
        auto player = dynamic_cast<GameRole*>(single);
        ZinxKernel::Zinx_SendOut(*pRetMsg, *player->m_protocol);
    }
}

float GameRole::GetX()
{
    return x;
}

float GameRole::GetY()
{
    return z;
}
