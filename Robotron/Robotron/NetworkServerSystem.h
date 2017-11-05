#pragma once

#include "NetworkSystem.h"
#include "PlayerInfo.h"
#include "ClientInfo.h"
#include "Audio.h"
#include "SceneEventListener.h"

#include <vector>
#include <unordered_map>
#include <memory>
#include <queue>
#include <string>

class Scene;
class Entity;
struct Packet;
struct sockaddr_in;
struct InputComponent;

enum ServerState {
	SERVER_STATE_LOBBY_MODE,
	SERVER_STATE_IN_GAME
};

class EntityPriorityComparitor {
public:
	bool operator()(const Entity* lhs, const Entity* rhs)
	{
		return lhs->network.priority < rhs->network.priority;
	}
};

class NetworkServerSystem : public NetworkSystem, public SceneEventListener {
public:
	NetworkServerSystem(Scene&, const std::string& username, const std::string& serverName);
	virtual ~NetworkServerSystem() override;

	virtual void beginFrame() override;
	virtual void update(Entity&, float deltaTick) override;
	virtual void endFrame() override;
	virtual bool isInGame() override;
	virtual void startGame() override;

	// Inherited via SceneEventListener
	virtual void onEntityCreation(Entity& entity) override;
	virtual void onEntityDestruction(Entity& entity) override;

	void recordInput(std::int32_t entityNetId, const InputComponent& input);
	void sendAudio(Sound);

	static const USHORT s_kDefaultServerPort;
private:

	std::string m_serverName;
	std::unordered_map<sockaddr_in, ClientInfo> m_clients;
	ServerState m_serverState;
	PlayerInfo m_serverPlayerInfo;
	Entity* m_serverPlayer;

	using SnapshotBufT = decltype(m_sendPacket.ghostSnapshotBuffer);

	void handleGamePackets(const Packet&, const sockaddr_in& address);
	void handleLobbyPackets(const Packet&, const sockaddr_in& address);
	void handleBroadcastPacket(const Packet&, const sockaddr_in& address);
	void handleJoinPacket(const Packet&, const sockaddr_in& address);
	std::vector<PlayerInfo> getPlayers();

	void broadcastToClients(const Packet& packet);

	// Selects 'maxSnapshots' entities from an array by their network priority
	// and places snapshots of them into the supplied snapshot buffer.
	// The buffer will be cleared before selecting snapshots into it.
	// Entities placed in the buffer will have their priorities reset to 0.
	void selectGhostSnapshots(SnapshotBufT& dst, std::vector<Entity*>& src,
	                          size_t maxSnapshots);
};

