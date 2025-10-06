#include "extension.h"

#define MAX_PROCESS 20

DiscordExtension g_DiscordExt;
SMEXT_LINK(&g_DiscordExt);

HandleType_t
	g_DiscordHandle,
	g_DiscordUserHandle,
	g_DiscordMessageHandle,
	g_DiscordChannelHandle,
	g_DiscordWebhookHandle,
	g_DiscordEmbedHandle,
	g_DiscordInteractionHandle,
	g_DiscordAutocompleteInteractionHandle,
	g_DiscordSlashCommandHandle,
	g_DiscordForumTagHandle,
	g_DiscordGuildHandle,
	g_DiscordRoleHandle,
	g_DiscordInviteHandle,
	g_HttpHeadersHandle,
	g_HttpCompletionHandle;

DiscordHandler g_DiscordHandler;
DiscordUserHandler g_DiscordUserHandler;
DiscordMessageHandler g_DiscordMessageHandler;
DiscordChannelHandler g_DiscordChannelHandler;
DiscordWebhookHandler g_DiscordWebhookHandler;
DiscordEmbedHandler g_DiscordEmbedHandler;
DiscordInteractionHandler g_DiscordInteractionHandler;
DiscordAutocompleteInteractionHandler g_DiscordAutocompleteInteractionHandler;
DiscordSlashCommandHandler g_DiscordSlashCommandHandler;
DiscordForumTagHandler g_DiscordForumTagHandler;
DiscordGuildHandler g_DiscordGuildHandler;
DiscordRoleHandler g_DiscordRoleHandler;
DiscordInviteHandler g_DiscordInviteHandler;
HttpHeadersHandler g_HttpHeadersHandler;
HttpCompletionHandler g_HttpCompletionHandler;

ThreadSafeQueue<std::function<void()>> g_TaskQueue;

static void OnGameFrame(bool simulating) {
	std::function<void()> task;
	int count = 0;
	while (g_TaskQueue.TryPop(task) && count < MAX_PROCESS) {
		task();
		count++;
	}
}

bool DiscordExtension::SDK_OnLoad(char* error, size_t maxlen, bool late)
{
	sharesys->AddNatives(myself, discord_natives);
	sharesys->RegisterLibrary(myself, "discord");

	HandleAccess haDefaults;
	handlesys->InitAccessDefaults(nullptr, &haDefaults);
	haDefaults.access[HandleAccess_Delete] = 0;

	g_DiscordHandle = handlesys->CreateType("Discord", &g_DiscordHandler, 0, nullptr, &haDefaults, myself->GetIdentity(), nullptr);
	g_DiscordUserHandle = handlesys->CreateType("DiscordUser", &g_DiscordUserHandler, 0, nullptr, &haDefaults, myself->GetIdentity(), nullptr);
	g_DiscordMessageHandle = handlesys->CreateType("DiscordMessage", &g_DiscordMessageHandler, 0, nullptr, &haDefaults, myself->GetIdentity(), nullptr);
	g_DiscordChannelHandle = handlesys->CreateType("DiscordChannel", &g_DiscordChannelHandler, 0, nullptr, &haDefaults, myself->GetIdentity(), nullptr);
	g_DiscordWebhookHandle = handlesys->CreateType("DiscordWebhook", &g_DiscordWebhookHandler, 0, nullptr, &haDefaults, myself->GetIdentity(), nullptr);
	g_DiscordEmbedHandle = handlesys->CreateType("DiscordEmbed", &g_DiscordEmbedHandler, 0, nullptr, &haDefaults, myself->GetIdentity(), nullptr);
	g_DiscordInteractionHandle = handlesys->CreateType("DiscordInteraction", &g_DiscordInteractionHandler, 0, nullptr, &haDefaults, myself->GetIdentity(), nullptr);
	g_DiscordAutocompleteInteractionHandle = handlesys->CreateType("DiscordAutocompleteInteraction", &g_DiscordAutocompleteInteractionHandler, 0, nullptr, &haDefaults, myself->GetIdentity(), nullptr);
	g_DiscordSlashCommandHandle = handlesys->CreateType("DiscordSlashCommand", &g_DiscordSlashCommandHandler, 0, nullptr, &haDefaults, myself->GetIdentity(), nullptr);
	g_DiscordForumTagHandle = handlesys->CreateType("DiscordForumTag", &g_DiscordForumTagHandler, 0, nullptr, &haDefaults, myself->GetIdentity(), nullptr);
	g_DiscordGuildHandle = handlesys->CreateType("DiscordGuild", &g_DiscordGuildHandler, 0, nullptr, &haDefaults, myself->GetIdentity(), nullptr);
	g_DiscordRoleHandle = handlesys->CreateType("DiscordRole", &g_DiscordRoleHandler, 0, nullptr, &haDefaults, myself->GetIdentity(), nullptr);
	g_DiscordInviteHandle = handlesys->CreateType("DiscordInvite", &g_DiscordInviteHandler, 0, nullptr, &haDefaults, myself->GetIdentity(), nullptr);
	g_HttpHeadersHandle = handlesys->CreateType("HttpHeaders", &g_HttpHeadersHandler, 0, nullptr, &haDefaults, myself->GetIdentity(), nullptr);
	g_HttpCompletionHandle = handlesys->CreateType("HttpCompletion", &g_HttpCompletionHandler, 0, nullptr, &haDefaults, myself->GetIdentity(), nullptr);

	smutils->AddGameFrameHook(&OnGameFrame);

	return true;
}

void DiscordExtension::SDK_OnUnload()
{
	handlesys->RemoveType(g_DiscordHandle, myself->GetIdentity());
	handlesys->RemoveType(g_DiscordUserHandle, myself->GetIdentity());
	handlesys->RemoveType(g_DiscordMessageHandle, myself->GetIdentity());
	handlesys->RemoveType(g_DiscordChannelHandle, myself->GetIdentity());
	handlesys->RemoveType(g_DiscordWebhookHandle, myself->GetIdentity());
	handlesys->RemoveType(g_DiscordEmbedHandle, myself->GetIdentity());
	handlesys->RemoveType(g_DiscordInteractionHandle, myself->GetIdentity());
	handlesys->RemoveType(g_DiscordAutocompleteInteractionHandle, myself->GetIdentity());
	handlesys->RemoveType(g_DiscordSlashCommandHandle, myself->GetIdentity());
	handlesys->RemoveType(g_DiscordForumTagHandle, myself->GetIdentity());
	handlesys->RemoveType(g_DiscordGuildHandle, myself->GetIdentity());
	handlesys->RemoveType(g_DiscordRoleHandle, myself->GetIdentity());
	handlesys->RemoveType(g_DiscordInviteHandle, myself->GetIdentity());
	handlesys->RemoveType(g_HttpHeadersHandle, myself->GetIdentity());
	handlesys->RemoveType(g_HttpCompletionHandle, myself->GetIdentity());

	smutils->RemoveGameFrameHook(&OnGameFrame);
}