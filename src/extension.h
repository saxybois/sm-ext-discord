#ifndef _INCLUDE_SOURCEMOD_EXTENSION_PROPER_H_
#define _INCLUDE_SOURCEMOD_EXTENSION_PROPER_H_

#include "smsdk_ext.h"
#include <queue>
#include <mutex>
#include <condition_variable>
#include <unordered_set>
#include "queue.h"
#include "dpp/dpp.h"
#include "discord.h"

/**
 * @brief HandleDestructor is a template class that is used to handle the destruction of objects.
 * 
 * @tparam T The type of the object
 */
template<typename T>
class HandleDestructor : public IHandleTypeDispatch
{
public:
	void OnHandleDestroy(HandleType_t type, void* object) override {
		delete static_cast<T*>(object);
	}
};

using DiscordHandler = HandleDestructor<DiscordClient>;
using DiscordUserHandler = HandleDestructor<DiscordUser>;
using DiscordMessageHandler = HandleDestructor<DiscordMessage>;
using DiscordChannelHandler = HandleDestructor<DiscordChannel>;
using DiscordWebhookHandler = HandleDestructor<DiscordWebhook>;
using DiscordEmbedHandler = HandleDestructor<DiscordEmbed>;
using DiscordInteractionHandler = HandleDestructor<DiscordInteraction>;
using DiscordAutocompleteInteractionHandler = HandleDestructor<DiscordAutocompleteInteraction>;
using DiscordSlashCommandHandler = HandleDestructor<DiscordSlashCommand>;
using DiscordForumTagHandler = HandleDestructor<DiscordForumTag>;
using DiscordGuildHandler = HandleDestructor<DiscordGuild>;
using DiscordRoleHandler = HandleDestructor<DiscordRole>;
using DiscordInviteHandler = HandleDestructor<DiscordInvite>;
using HttpHeadersHandler = HandleDestructor<HttpHeaders>;
using HttpCompletionHandler = HandleDestructor<HttpCompletion>;

class DiscordExtension : public SDKExtension
{
public:
	virtual bool SDK_OnLoad(char* error, size_t maxlength, bool late);
	virtual void SDK_OnUnload();
};

extern DiscordExtension g_DiscordExt;
extern ThreadSafeQueue<std::function<void()>> g_TaskQueue;

extern HandleType_t
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

extern DiscordHandler g_DiscordHandler;
extern DiscordUserHandler g_DiscordUserHandler;
extern DiscordMessageHandler g_DiscordMessageHandler;
extern DiscordChannelHandler g_DiscordChannelHandler;
extern DiscordWebhookHandler g_DiscordWebhookHandler;
extern DiscordEmbedHandler g_DiscordEmbedHandler;
extern DiscordInteractionHandler g_DiscordInteractionHandler;
extern DiscordAutocompleteInteractionHandler g_DiscordAutocompleteInteractionHandler;
extern DiscordSlashCommandHandler g_DiscordSlashCommandHandler;
extern DiscordForumTagHandler g_DiscordForumTagHandler;
extern DiscordGuildHandler g_DiscordGuildHandler;
extern DiscordRoleHandler g_DiscordRoleHandler;
extern DiscordInviteHandler g_DiscordInviteHandler;
extern HttpHeadersHandler g_HttpHeadersHandler;
extern HttpCompletionHandler g_HttpCompletionHandler;

extern const sp_nativeinfo_t discord_natives[];

#endif // _INCLUDE_SOURCEMOD_EXTENSION_PROPER_H_
