#pragma once

#include <vector>
#include <memory>
#include <functional>

namespace INF4215_TP3
{
    class ITile;
    class Room;
    class Player;

    namespace Ketrud
    {
        struct PlayerInfo;

        class StateGame
        {
        public:
            StateGame(const Room&, const Player& player/*, const Player& otherPlayer*/);
            ~StateGame();

            bool operator==(const StateGame&) const;

            const std::vector<std::shared_ptr<const ITile>>& GetTiles() const
            {
                return m_vecTiles;
            }

            const PlayerInfo& GetPlayerInfo() const
            {
                return *m_pPlayer;
            }

            /*
            const PlayerInfo& GetOtherPlayerInfo() const
            {
                return *m_pOtherPlayer;
            }
            */
            bool IsTerminal() const;

        private:
            std::vector<std::shared_ptr<const ITile>> m_vecTiles;
            std::unique_ptr<const PlayerInfo> m_pPlayer;
            //std::unique_ptr<const PlayerInfo> m_pOtherPlayer;
        };
    }
}

namespace std
{
    template<>
    struct hash<INF4215_TP3::Ketrud::StateGame>
    {
        size_t operator()(const INF4215_TP3::Ketrud::StateGame&) const noexcept;
    };
}

