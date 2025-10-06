#ifndef _INCLUDE_DISCORD_H_
#define _INCLUDE_DISCORD_H_

#include "extension.h"

/**
 * @brief Get the pointer to the object from the handle
 * 
 * @tparam T The type of the object
 * @param pContext The plugin context
 * @param handle The handle to the object
 * @param handleType The type of the handle
 * @param typeName The name of the type
 * @return The pointer to the object
 */
template<typename T>
static T* GetHandlePointer(IPluginContext* pContext, Handle_t handle, HandleType_t handleType, const char* typeName) {
	HandleError err;
	HandleSecurity sec(pContext->GetIdentity(), myself->GetIdentity());

	T* object;
	if ((err = handlesys->ReadHandle(handle, handleType, &sec, (void**)&object)) != HandleError_None) {
		pContext->ReportError("Invalid %s handle %x (error %d)", typeName, handle, err);
		return nullptr;
	}

	return object;
}

class DiscordClient;
class DiscordInvite;

class DiscordEmbed
{
private:
	dpp::embed m_embed;

public:
	DiscordEmbed() {}

	void SetTitle(const char* title) { m_embed.set_title(title); }
	void SetDescription(const char* desc) { m_embed.set_description(desc); }
	void SetColor(uint32_t color) { m_embed.set_color(color); }
	void SetUrl(const char* url) { m_embed.set_url(url); }
	void SetAuthor(const char* name, const char* url = nullptr, const char* icon_url = nullptr) {
		m_embed.set_author(name, url ? url : "", icon_url ? icon_url : "");
	}
	void SetFooter(const char* text, const char* icon_url = nullptr) {
		m_embed.set_footer(text, icon_url ? icon_url : "");
	}
	void AddField(const char* name, const char* value, bool inLine = false) {
		m_embed.add_field(name, value, inLine);
	}
	void SetThumbnail(const char* url) { m_embed.set_thumbnail(url); }
	void SetImage(const char* url) { m_embed.set_image(url); }
	void SetTimestamp(time_t timestamp) { m_embed.set_timestamp(timestamp); }
	void SetVideo(const char* url) { m_embed.set_video(url); }
	void SetProvider(const char* name, const char* url = nullptr) {
		m_embed.set_provider(name, url ? url : "");
	}

	// Get methods
	std::string GetTitle() const { return m_embed.title; }
	std::string GetDescription() const { return m_embed.description; }
	uint32_t GetColor() const { return m_embed.color.value_or(0); }
	time_t GetTimestamp() const { return m_embed.timestamp; }
	std::string GetType() const { return m_embed.type; }
	std::string GetUrl() const { return m_embed.url; }
	std::string GetAuthorName() const { return m_embed.author.has_value() ? m_embed.author->name : ""; }
	std::string GetAuthorUrl() const { return m_embed.author.has_value() ? m_embed.author->url : ""; }
	std::string GetAuthorIconUrl() const { return m_embed.author.has_value() ? m_embed.author->icon_url : ""; }
	std::string GetAuthorProxyIconUrl() const { return m_embed.author.has_value() ? m_embed.author->proxy_icon_url : ""; }
	std::string GetFooterText() const { return m_embed.footer.has_value() ? m_embed.footer->text : ""; }
	std::string GetFooterIconUrl() const { return m_embed.footer.has_value() ? m_embed.footer->icon_url : ""; }
	std::string GetFooterProxyUrl() const { return m_embed.footer.has_value() ? m_embed.footer->proxy_url : ""; }
	std::string GetThumbnailUrl() const { return m_embed.thumbnail.has_value() ? m_embed.thumbnail->url : ""; }
	std::string GetThumbnailProxyUrl() const { return m_embed.thumbnail.has_value() ? m_embed.thumbnail->proxy_url : ""; }
	uint32_t GetThumbnailWidth() const { return m_embed.thumbnail.has_value() ? m_embed.thumbnail->width : 0; }
	uint32_t GetThumbnailHeight() const { return m_embed.thumbnail.has_value() ? m_embed.thumbnail->height : 0; }
	std::string GetImageUrl() const { return m_embed.image.has_value() ? m_embed.image->url : ""; }
	std::string GetImageProxyUrl() const { return m_embed.image.has_value() ? m_embed.image->proxy_url : ""; }
	uint32_t GetImageWidth() const { return m_embed.image.has_value() ? m_embed.image->width : 0; }
	uint32_t GetImageHeight() const { return m_embed.image.has_value() ? m_embed.image->height : 0; }
	std::string GetVideoUrl() const { return m_embed.video.has_value() ? m_embed.video->url : ""; }
	std::string GetVideoProxyUrl() const { return m_embed.video.has_value() ? m_embed.video->proxy_url : ""; }
	uint32_t GetVideoWidth() const { return m_embed.video.has_value() ? m_embed.video->width : 0; }
	uint32_t GetVideoHeight() const { return m_embed.video.has_value() ? m_embed.video->height : 0; }
	std::string GetProviderName() const { return m_embed.provider.has_value() ? m_embed.provider->name : ""; }
	std::string GetProviderUrl() const { return m_embed.provider.has_value() ? m_embed.provider->url : ""; }
	size_t GetFieldCount() const { return m_embed.fields.size(); }
	std::string GetFieldName(size_t index) const { return index < m_embed.fields.size() ? m_embed.fields[index].name : ""; }
	std::string GetFieldValue(size_t index) const { return index < m_embed.fields.size() ? m_embed.fields[index].value : ""; }
	bool GetFieldInline(size_t index) const { return index < m_embed.fields.size() ? m_embed.fields[index].is_inline : false; }

	// Field management
	bool RemoveField(size_t index) {
		if (index < m_embed.fields.size()) {
			m_embed.fields.erase(m_embed.fields.begin() + index);
			return true;
		}
		return false;
	}
	void ClearFields() { m_embed.fields.clear(); }

	// Utility properties
	bool HasThumbnail() const { return m_embed.thumbnail.has_value(); }
	bool HasImage() const { return m_embed.image.has_value(); }
	bool HasVideo() const { return m_embed.video.has_value(); }
	bool HasProvider() const { return m_embed.provider.has_value(); }
	bool HasAuthor() const { return m_embed.author.has_value(); }
	bool HasFooter() const { return m_embed.footer.has_value(); }
	bool HasTimestamp() const { return m_embed.timestamp != 0; }

	const dpp::embed& GetEmbed() const { return m_embed; }
};

class DiscordForumTag
{
private:
	dpp::forum_tag m_tag;

public:
	DiscordForumTag() {}
	DiscordForumTag(const char* name, const char* emoji = "", bool moderated = false) {
		m_tag.set_name(name);
		if (emoji && strlen(emoji) > 0) {
			// Check if emoji is a snowflake ID (numeric) or unicode
			if (std::all_of(emoji, emoji + strlen(emoji), ::isdigit)) {
				m_tag.emoji = dpp::snowflake(emoji);
			} else {
				m_tag.emoji = std::string(emoji);
			}
		}
		m_tag.moderated = moderated;
	}
	DiscordForumTag(const dpp::forum_tag& tag) : m_tag(tag) {}

	std::string GetId() const { return m_tag.id.str(); }
	std::string GetName() const { return m_tag.name; }
	void SetName(const char* name) { m_tag.set_name(name); }

	std::string GetEmoji() const {
		if (std::holds_alternative<dpp::snowflake>(m_tag.emoji)) {
			return std::get<dpp::snowflake>(m_tag.emoji).str();
		} else if (std::holds_alternative<std::string>(m_tag.emoji)) {
			return std::get<std::string>(m_tag.emoji);
		}
		return "";
	}

	void SetEmoji(const char* emoji) {
		if (emoji && strlen(emoji) > 0) {
			// Check if emoji is a snowflake ID (numeric) or unicode
			if (std::all_of(emoji, emoji + strlen(emoji), ::isdigit)) {
				m_tag.emoji = dpp::snowflake(emoji);
			} else {
				m_tag.emoji = std::string(emoji);
			}
		} else {
			m_tag.emoji = std::monostate{};
		}
	}

	bool IsModerated() const { return m_tag.moderated; }
	void SetModerated(bool moderated) { m_tag.moderated = moderated; }

	bool EmojiIsCustom() const {
		return std::holds_alternative<dpp::snowflake>(m_tag.emoji);
	}

	const dpp::forum_tag& GetTag() const { return m_tag; }
};

class DiscordUser
{
private:
	dpp::user m_user;
	dpp::guild_member m_member;
	bool m_has_member;
	DiscordClient* m_client;

public:
	DiscordUser(const dpp::user& user) : m_user(user), m_has_member(false), m_client(nullptr) {}
	DiscordUser(const dpp::user& user, DiscordClient* client) : m_user(user), m_has_member(false), m_client(client) {}
	DiscordUser(const dpp::user& user, const dpp::guild_member& member) : m_user(user), m_member(member), m_has_member(true), m_client(nullptr) {}
	DiscordUser(const dpp::user& user, const dpp::guild_member& member, DiscordClient* client) : m_user(user), m_member(member), m_has_member(true), m_client(client) {}

	std::string GetId() const { return m_user.id.str(); }
	const char* GetUserName() const { return m_user.username.c_str(); }
	const uint16_t GetDiscriminator() const { return m_user.discriminator; }
	const char* GetGlobalName() const { return m_user.global_name.c_str(); }
	std::string GetAvatarUrl(bool prefer_animated_avatars) const { return m_user.get_avatar_url(0, dpp::i_png, prefer_animated_avatars); }
	std::string GetDefaultAvatarUrl() const { return m_user.get_default_avatar_url(); }
	std::string GetAvatarDecorationUrl(uint16_t size = 0) const { return m_user.get_avatar_decoration_url(size); }
	std::string GetMention() const { return m_user.get_mention(); }
	std::string GetUrl() const { return m_user.get_url(); }
	std::string FormatUsername() const { return m_user.format_username(); }
	bool IsBot() const { return m_user.is_bot(); }
	uint32_t GetFlags() const { return m_user.flags; }
	bool HasFlag(uint32_t flag) const { return (m_user.flags & flag) != 0; }

	// Guild member specific methods
	bool HasGuildMember() const { return m_has_member; }
	std::string GetNickName() const { return m_has_member ? m_member.get_nickname() : ""; }
	time_t GetJoinedAt() const { return m_has_member ? m_member.joined_at : 0; }
	bool IsPending() const { return m_has_member ? m_member.is_pending() : false; }

	// Permission methods
	bool HasPermission(const char* permission) const;
	uint64_t GetPermissions() const;
	bool HasPermissionInChannel(dpp::snowflake channel_id, const char* permission) const;
	uint64_t GetPermissionsInChannel(dpp::snowflake channel_id) const;

	// Role methods
	std::vector<dpp::snowflake> GetRoles() const;
	bool HasRole(dpp::snowflake role_id) const;
	bool HasAnyRole(const std::vector<dpp::snowflake>& role_ids) const;
	bool HasAllRoles(const std::vector<dpp::snowflake>& role_ids) const;
	dpp::snowflake GetHighestRole() const;
	std::string GetRoleName(dpp::snowflake role_id) const;
	std::vector<std::string> GetRoleNames() const;

	// Member management methods
	bool SetNickName(const char* nickname);
	bool AddRole(dpp::snowflake role_id);
	bool RemoveRole(dpp::snowflake role_id);
	bool KickFromGuild();
	bool BanFromGuild(const char* reason = nullptr, int delete_message_days = 0);
	bool UnbanFromGuild();
	bool SetTimeout(time_t timeout_until);
	bool RemoveTimeout();

	// Internal accessors
	const dpp::user& GetDPPUser() const { return m_user; }
	const dpp::guild_member& GetDPPMember() const { return m_member; }
	DiscordClient* GetClient() const { return m_client; }
	dpp::snowflake GetGuildId() const {
		if (m_has_member) {
			return m_member.guild_id;
		}
		return 0;
	}
};

class DiscordMessage
{
private:
	dpp::message m_message;
	DiscordClient* m_client;
	mutable Handle_t m_authorHandle;

public:
	DiscordMessage(const dpp::message& msg, DiscordClient* client) : m_message(msg), m_client(client), m_authorHandle(BAD_HANDLE) {}
	DiscordMessage(DiscordClient* client) : m_client(client), m_authorHandle(BAD_HANDLE) {} // Empty message
	DiscordMessage(const char* content, DiscordClient* client) : m_client(client), m_authorHandle(BAD_HANDLE) { 
		m_message.content = content; 
	}
	DiscordMessage(dpp::snowflake channel_id, const char* content, DiscordClient* client) : m_client(client), m_authorHandle(BAD_HANDLE) { 
		m_message.channel_id = channel_id;
		m_message.content = content; 
	}
	DiscordMessage(const DiscordEmbed* embed, DiscordClient* client) : m_client(client), m_authorHandle(BAD_HANDLE) { 
		if (embed) m_message.add_embed(embed->GetEmbed()); 
	}
	DiscordMessage(dpp::snowflake channel_id, const DiscordEmbed* embed, DiscordClient* client) : m_client(client), m_authorHandle(BAD_HANDLE) { 
		m_message.channel_id = channel_id;
		if (embed) m_message.add_embed(embed->GetEmbed()); 
	}

	~DiscordMessage() {
		if (m_authorHandle) {
			handlesys->FreeHandle(m_authorHandle, nullptr);
		}
	}

	Handle_t GetAuthorHandle() const;

	DiscordUser* GetAuthor() const { 
		if (m_message.guild_id != 0 && m_message.member.user_id != 0) {
			return new DiscordUser(m_message.author, m_message.member, m_client);
		}
		return new DiscordUser(m_message.author, m_client); 
	}
	const char* GetContent() const { return m_message.content.c_str(); }
	const size_t GetContentLength() const { return m_message.content.length(); }
	std::string GetMessageId() const { return m_message.id.str(); }
	std::string GetChannelId() const { return m_message.channel_id.str(); }
	std::string GetGuildId() const { return m_message.guild_id.str(); }
	std::string GetAuthorNickname() const { return m_message.member.get_nickname(); }
	dpp::message_type GetType() const { return m_message.type; }
	bool IsPinned() const { return m_message.pinned; }
	bool IsTTS() const { return m_message.tts; }
	bool IsMentionEveryone() const { return m_message.mention_everyone; }
	bool IsBot() const { return m_message.author.is_bot(); }

	// Message flags and properties
	uint16_t GetFlags() const { return m_message.flags; }
	bool IsCrossposted() const { return (m_message.flags & (1 << 0)) != 0; }
	bool IsCrosspost() const { return (m_message.flags & (1 << 1)) != 0; }
	bool EmbedsSuppressed() const { return (m_message.flags & (1 << 2)) != 0; }
	bool IsSourceMessageDeleted() const { return (m_message.flags & (1 << 3)) != 0; }
	bool IsUrgent() const { return (m_message.flags & (1 << 4)) != 0; }
	bool HasThread() const { return (m_message.flags & (1 << 5)) != 0; }
	bool IsEphemeral() const { return (m_message.flags & (1 << 6)) != 0; }
	bool IsLoading() const { return (m_message.flags & (1 << 7)) != 0; }
	bool IsThreadMentionFailed() const { return (m_message.flags & (1 << 8)) != 0; }
	bool NotificationsSuppressed() const { return (m_message.flags & (1 << 12)) != 0; }
	bool IsVoiceMessage() const { return (m_message.flags & (1 << 13)) != 0; }
	bool HasSnapshot() const { return (m_message.flags & (1 << 14)) != 0; }
	bool IsUsingComponentsV2() const { return (m_message.flags & (1 << 15)) != 0; }

	// Additional properties
	std::string GetWebhookId() const { return m_message.webhook_id.str(); }
	time_t GetTimestamp() const { return m_message.sent; }
	time_t GetEditedTimestamp() const { return m_message.edited; }
	const char* GetNonce() const { return m_message.nonce.c_str(); }
	bool IsDM() const { return m_message.guild_id == 0; }
	bool HasRemixAttachment() const {
		for (const auto& attachment : m_message.attachments) {
			if (attachment.flags & (1 << 2)) return true; // a_is_remix
		}
		return false;
	}
	std::string GetURL() const {
		if (m_message.guild_id != 0) {
			return "https://discord.com/channels/" + m_message.guild_id.str() + "/" + 
				m_message.channel_id.str() + "/" + m_message.id.str();
		} else {
			return "https://discord.com/channels/@me/" + m_message.channel_id.str() + "/" + 
				m_message.id.str();
		}
	}

	// Collection counts
	size_t GetAttachmentCount() const { return m_message.attachments.size(); }
	size_t GetEmbedCount() const { return m_message.embeds.size(); }
	size_t GetReactionCount() const { return m_message.reactions.size(); }
	size_t GetStickerCount() const { return m_message.stickers.size(); }
	size_t GetMentionedUserCount() const { return m_message.mentions.size(); }
	size_t GetMentionedRoleCount() const { return m_message.mention_roles.size(); }
	size_t GetMentionedChannelCount() const { return m_message.mention_channels.size(); }

	// Collection accessors
	std::string GetAttachmentFilename(size_t index) const {
		if (index >= m_message.attachments.size()) return "";
		return m_message.attachments[index].filename;
	}
	std::string GetAttachmentURL(size_t index) const {
		if (index >= m_message.attachments.size()) return "";
		return m_message.attachments[index].url;
	}
	uint32_t GetAttachmentSize(size_t index) const {
		if (index >= m_message.attachments.size()) return 0;
		return m_message.attachments[index].size;
	}
	std::string GetMentionedUserId(size_t index) const {
		if (index >= m_message.mentions.size()) return "";
		return m_message.mentions[index].first.id.str();
	}
	std::string GetMentionedRoleId(size_t index) const {
		if (index >= m_message.mention_roles.size()) return "";
		return m_message.mention_roles[index].str();
	}

	// Message actions
	bool Reply(const char* content);
	bool ReplyEmbed(const char* content, const class DiscordEmbed* embed);
	bool Crosspost();
	bool CreateThread(const char* name, int auto_archive_duration = 60);

	// Message management methods
	bool Edit(const char* new_content);
	bool EditEmbed(const char* new_content, const class DiscordEmbed* embed);
	bool Delete();
	bool Pin();
	bool Unpin();
	bool AddReaction(const char* emoji);
	bool RemoveReaction(const char* emoji);
	bool RemoveAllReactions();

	// Message property setters
	void SetContent(const char* content) { m_message.content = content; }
	void SetChannelId(dpp::snowflake channel_id) { m_message.channel_id = channel_id; }
	void SetType(dpp::message_type type) { m_message.type = type; }
	void SetFlags(uint16_t flags) { m_message.flags = flags; }
	void SetTTS(bool tts) { m_message.tts = tts; }
	void SetNonce(const char* nonce) { m_message.nonce = nonce; }
	void SetAllowedMentions(int allowed_mentions_mask, const std::vector<dpp::snowflake>& users, const std::vector<dpp::snowflake>& roles) {
		m_message.set_allowed_mentions(
			allowed_mentions_mask & 1,
			allowed_mentions_mask & 2,
			allowed_mentions_mask & 4,
			allowed_mentions_mask & 8,
			users,
			roles
		);
	}
	void AddEmbed(const class DiscordEmbed* embed) { 
		if (embed) m_message.add_embed(embed->GetEmbed()); 
	}
	void ClearEmbeds() { m_message.embeds.clear(); }
	bool Send(); // Send this message to its channel

	// Internal accessor
	const dpp::message& GetDPPMessage() const { return m_message; }
};

class DiscordChannel
{
private:
	dpp::channel m_channel;
	DiscordClient* m_client;

public:
	DiscordChannel(const dpp::channel& chnl) : m_channel(chnl), m_client(nullptr) {}
	DiscordChannel(const dpp::channel& chnl, DiscordClient* client) : m_channel(chnl), m_client(client) {}

	// Basic information
	const char* GetName() const { return m_channel.name.c_str(); }
	std::string GetId() const { return m_channel.id.str(); }
	std::string GetGuildId() const { return m_channel.guild_id.str(); }
	std::string GetParentId() const { return m_channel.parent_id.str(); }
	void SetParentId(const char* parent_id) { m_channel.parent_id = parent_id; }
	const char* GetTopic() const { return m_channel.topic.c_str(); }

	// Channel type and properties
	uint8_t GetType() const { return static_cast<uint8_t>(m_channel.get_type()); }
	uint16_t GetPosition() const { return m_channel.position; }
	bool IsNSFW() const { return m_channel.is_nsfw(); }
	bool IsTextChannel() const { return m_channel.is_text_channel(); }
	bool IsVoiceChannel() const { return m_channel.is_voice_channel(); }
	bool IsCategory() const { return m_channel.is_category(); }
	bool IsThread() const { 
		dpp::channel_type type = m_channel.get_type();
		return type == dpp::CHANNEL_ANNOUNCEMENT_THREAD || 
			type == dpp::CHANNEL_PUBLIC_THREAD || 
			type == dpp::CHANNEL_PRIVATE_THREAD;
	}
	bool IsForum() const { return m_channel.is_forum(); }
	bool IsNewsChannel() const { return m_channel.is_news_channel(); }
	bool IsStageChannel() const { return m_channel.is_stage_channel(); }

	// Voice channel specific
	uint16_t GetBitrate() const { return m_channel.bitrate; }
	uint8_t GetUserLimit() const { return m_channel.user_limit; }

	// Text channel specific
	uint16_t GetRateLimitPerUser() const { return m_channel.rate_limit_per_user; }

	// Additional channel properties
	uint16_t GetFlags() const { return m_channel.flags; }
	std::string GetOwnerId() const { return m_channel.owner_id.str(); }
	std::string GetLastMessageId() const { return m_channel.last_message_id.str(); }
	time_t GetLastPinTimestamp() const { return m_channel.last_pin_timestamp; }
	uint16_t GetDefaultThreadRateLimitPerUser() const { return m_channel.default_thread_rate_limit_per_user; }
	uint8_t GetDefaultAutoArchiveDuration() const { return static_cast<uint8_t>(m_channel.default_auto_archive_duration); }
	uint8_t GetDefaultSortOrder() const { return static_cast<uint8_t>(m_channel.default_sort_order); }
	uint8_t GetForumLayout() const { return static_cast<uint8_t>(m_channel.get_default_forum_layout()); }
	const char* GetRTCRegion() const { return m_channel.rtc_region.c_str(); }

	// Additional channel type checks
	bool IsDM() const { return m_channel.is_dm(); }
	bool IsGroupDM() const { return m_channel.is_group_dm(); }
	bool IsMediaChannel() const { return m_channel.is_media_channel(); }
	bool IsVideo720p() const { return m_channel.is_video_720p(); }
	bool IsVideoAuto() const { return m_channel.is_video_auto(); }
	bool IsPinnedThread() const { return m_channel.is_pinned_thread(); }
	bool IsTagRequired() const { return m_channel.is_tag_required(); }
	bool IsDownloadOptionsHidden() const { return m_channel.is_download_options_hidden(); }
	bool IsLockedPermissions() const { return m_channel.is_locked_permissions(); }

	// URLs and mentions
	std::string GetMention() const { return m_channel.get_mention(); }
	std::string GetUrl() const { return m_channel.get_url(); }
	std::string GetIconUrl(uint16_t size = 0) const { return m_channel.get_icon_url(size); }

	// Permission overwrites
	size_t GetPermissionOverwriteCount() const { return m_channel.permission_overwrites.size(); }
	std::string GetPermissionOverwriteTargetId(size_t index) const {
		if (index >= m_channel.permission_overwrites.size()) return "";
		return m_channel.permission_overwrites[index].id.str();
	}
	uint8_t GetPermissionOverwriteType(size_t index) const {
		if (index >= m_channel.permission_overwrites.size()) return 0;
		return m_channel.permission_overwrites[index].type;
	}

	// Forum tags
	size_t GetAvailableTagCount() const { return m_channel.available_tags.size(); }
	std::string GetAvailableTagName(size_t index) const {
		if (index >= m_channel.available_tags.size()) return "";
		return m_channel.available_tags[index].name;
	}
	std::string GetAvailableTagId(size_t index) const {
		if (index >= m_channel.available_tags.size()) return "";
		return m_channel.available_tags[index].id.str();
	}

	// New Forum tag methods
	std::string GetAvailableTagEmoji(size_t index) const {
		if (index >= m_channel.available_tags.size()) return "";
		const auto& tag = m_channel.available_tags[index];
		if (std::holds_alternative<dpp::snowflake>(tag.emoji)) {
			return std::get<dpp::snowflake>(tag.emoji).str();
		} else if (std::holds_alternative<std::string>(tag.emoji)) {
			return std::get<std::string>(tag.emoji);
		}
		return "";
	}

	bool GetAvailableTagModerated(size_t index) const {
		if (index >= m_channel.available_tags.size()) return false;
		return m_channel.available_tags[index].moderated;
	}

	bool GetAvailableTagEmojiIsCustom(size_t index) const {
		if (index >= m_channel.available_tags.size()) return false;
		return std::holds_alternative<dpp::snowflake>(m_channel.available_tags[index].emoji);
	}

	// Forum tag management
	bool CreateForumTag(const char* name, const char* emoji = "", bool moderated = false);
	bool EditForumTag(dpp::snowflake tag_id, const char* name, const char* emoji = "", bool moderated = false);
	bool DeleteForumTag(dpp::snowflake tag_id);

	// Forum thread creation
	bool CreateForumThread(const char* name, const char* message, const std::vector<dpp::snowflake>& tag_ids = {}, int auto_archive = 1440, int rate_limit = 0);
	bool CreateForumThreadEmbed(const char* name, const char* message, const class DiscordEmbed* embed, const std::vector<dpp::snowflake>& tag_ids = {}, int auto_archive = 1440, int rate_limit = 0);

	// Permission management
	bool AddPermissionOverwrite(dpp::snowflake target_id, uint8_t type, uint64_t allowed, uint64_t denied);
	bool SetPermissionOverwrite(dpp::snowflake target_id, uint8_t type, uint64_t allowed, uint64_t denied);
	bool RemovePermissionOverwrite(dpp::snowflake target_id, uint8_t type);
	std::string GetUserPermissions(dpp::snowflake user_id) const;

	// Channel actions
	bool CreateInvite(int max_age = 86400, int max_uses = 0, bool temporary = false, bool unique = false);
	bool CreateInviteFromObject(const DiscordInvite* invite_obj);
	bool GetInvites(IForward* callback_forward, cell_t data = 0);
	bool GetWebhooks(IForward* callback_forward, cell_t data = 0);
	bool SendMessage(const char* content);
	bool SendMessageEmbed(const char* content, const class DiscordEmbed* embed);
	bool SendDiscordMessage(const class DiscordMessage* message);
	bool SetRTCRegion(const char* region);

	// Permission checking
	bool HasUserPermission(const dpp::user& user, const char* permission) const;
	bool HasMemberPermission(const dpp::guild_member& member, const char* permission) const;

	// Channel management methods
	bool SetName(const char* name);
	bool SetTopic(const char* topic);
	bool SetPosition(uint16_t position);
	bool SetNSFW(bool nsfw);
	bool SetRateLimitPerUser(uint16_t seconds);
	bool SetBitrate(uint16_t bitrate);
	bool SetUserLimit(uint8_t limit);
	bool Delete();
	bool SetParent(dpp::snowflake parent_id);

	// Internal accessor
	const dpp::channel& GetDPPChannel() const { return m_channel; }
};

class DiscordWebhook
{
private:
	mutable Handle_t m_userHandle;

public:
	dpp::webhook m_webhook;
	DiscordClient* m_client;

	// Constructors
	DiscordWebhook(const dpp::webhook& wbhk, DiscordClient* client) : m_userHandle(BAD_HANDLE), m_webhook(wbhk), m_client(client) {}
	DiscordWebhook(const std::string& webhook_url, DiscordClient* client = nullptr) : m_userHandle(BAD_HANDLE), m_webhook(webhook_url), m_client(client) {}
	DiscordWebhook(const char* webhook_url, DiscordClient* client = nullptr) : m_userHandle(BAD_HANDLE), m_webhook(std::string(webhook_url)), m_client(client) {}
	DiscordWebhook(dpp::snowflake webhook_id, const std::string& webhook_token, DiscordClient* client = nullptr) 
		: m_userHandle(BAD_HANDLE), m_client(client) { 
		m_webhook.id = webhook_id; 
		m_webhook.token = webhook_token; 
	}

	~DiscordWebhook() {
		if (m_userHandle) {
			handlesys->FreeHandle(m_userHandle, nullptr);
		}
	}

	// Basic properties
	std::string GetId() const { return m_webhook.id.str(); }

	Handle_t GetUserHandle() const;

	DiscordUser* GetUser() const { return new DiscordUser(m_webhook.user_obj, m_client); }
	const char* GetName() const { return m_webhook.name.c_str(); }
	void SetName(const char* value) { m_webhook.name = value; }
	const char* GetAvatarUrl() const { return m_webhook.avatar_url.c_str(); }
	void SetAvatarUrl(const char* value) { m_webhook.avatar_url = value; }
	std::string GetAvatarData() const { return m_webhook.avatar.to_string(); }
	void SetAvatarData(const char* value) { m_webhook.avatar = dpp::utility::iconhash(value); }

	// Additional webhook properties (read-only)
	uint8_t GetType() const { return m_webhook.type; }
	std::string GetGuildId() const { return m_webhook.guild_id.str(); }
	std::string GetChannelId() const { return m_webhook.channel_id.str(); }
	const char* GetToken() const { return m_webhook.token.c_str(); }
	std::string GetApplicationId() const { return m_webhook.application_id.str(); }
	std::string GetSourceGuildId() const { return m_webhook.source_guild.id.str(); }
	std::string GetSourceChannelId() const { return m_webhook.source_channel.id.str(); }
	const char* GetUrl() const { return m_webhook.url.c_str(); }
	std::string GetImageData() const { return m_webhook.image_data; }

	// Webhook management methods
	bool Modify();
	bool Delete();
	bool Execute(const char* message, int allowed_mentions_mask = 0, std::vector<dpp::snowflake> users = {}, std::vector<dpp::snowflake> roles = {});
	bool ExecuteEmbed(const char* message, const class DiscordEmbed* embed, int allowed_mentions_mask = 0, std::vector<dpp::snowflake> users = {}, std::vector<dpp::snowflake> roles = {});

	// Static webhook operations
	static bool CreateWebhook(DiscordClient* discord, dpp::webhook wh, IForward *callback_forward, cell_t data);
	static bool GetWebhook(DiscordClient* discord, dpp::snowflake webhook_id, IForward* callback_forward, cell_t data);
	static bool GetChannelWebhooks(DiscordClient* discord, dpp::snowflake channel_id, IForward *callback_forward, cell_t data);
	static bool GetGuildWebhooks(DiscordClient* discord, dpp::snowflake guild_id, IForward *callback_forward, cell_t data);

	// Internal accessor
	const dpp::webhook& GetDPPWebhook() const { return m_webhook; }
};

class DiscordClient
{
private:
	std::unique_ptr<dpp::cluster> m_cluster;
	std::unique_ptr<std::thread> m_thread;
	Handle_t m_discord_handle;

	struct CallbackData {
		IChangeableForward* forward;
		bool autoFreeHandles;
		cell_t data;
		CallbackData() : forward(nullptr), autoFreeHandles(true), data(0) {}
	};

	CallbackData m_readyCallback;
	CallbackData m_resumedCallback;
	CallbackData m_messageCallback;
	CallbackData m_logCallback;
	CallbackData m_slashCommandCallback;
	CallbackData m_autocompleteCallback;
	CallbackData m_shutdownCallback;

	void RunBot();
	void SetupEventHandlers();

public:
	DiscordClient(const char* token, uint32_t intents = 0);
	~DiscordClient();

	bool Initialize();
	void Start();
	void Stop();
	bool IsRunning() const;
	void SendClosePacket();
	void SetHandle(Handle_t handle) { 
		m_discord_handle = handle; 
	}
	Handle_t GetHandle() const { return m_discord_handle; }

	// Per-instance forward callback registration methods
	void SetReadyCallback(IChangeableForward* forward, cell_t data = 0) { 
		if (m_readyCallback.forward) forwards->ReleaseForward(m_readyCallback.forward);
		m_readyCallback.forward = forward;
		m_readyCallback.data = data;
	}
	void SetResumedCallback(IChangeableForward* forward, cell_t data = 0) { 
		if (m_resumedCallback.forward) forwards->ReleaseForward(m_resumedCallback.forward);
		m_resumedCallback.forward = forward;
		m_resumedCallback.data = data;
	}
	void SetMessageCallback(IChangeableForward* forward, bool autoFreeHandles = true, cell_t data = 0) { 
		if (m_messageCallback.forward) forwards->ReleaseForward(m_messageCallback.forward);
		m_messageCallback.forward = forward; 
		m_messageCallback.autoFreeHandles = autoFreeHandles; 
		m_messageCallback.data = data;
	}
	void SetLogCallback(IChangeableForward* forward, cell_t data = 0) { 
		if (m_logCallback.forward) forwards->ReleaseForward(m_logCallback.forward);
		m_logCallback.forward = forward; 
		m_logCallback.data = data;
	}
	void SetSlashCommandCallback(IChangeableForward* forward, bool autoFreeHandles = true, cell_t data = 0) { 
		if (m_slashCommandCallback.forward) forwards->ReleaseForward(m_slashCommandCallback.forward);
		m_slashCommandCallback.forward = forward; 
		m_slashCommandCallback.autoFreeHandles = autoFreeHandles; 
		m_slashCommandCallback.data = data;
	}
	void SetAutocompleteCallback(IChangeableForward* forward, bool autoFreeHandles = true, cell_t data = 0) { 
		if (m_autocompleteCallback.forward) forwards->ReleaseForward(m_autocompleteCallback.forward);
		m_autocompleteCallback.forward = forward; 
		m_autocompleteCallback.autoFreeHandles = autoFreeHandles; 
		m_autocompleteCallback.data = data;
	}
	void SetShutdownCallback(IChangeableForward* forward, cell_t data = 0) { 
		if (m_shutdownCallback.forward) forwards->ReleaseForward(m_shutdownCallback.forward);
		m_shutdownCallback.forward = forward; 
		m_shutdownCallback.data = data;
	}

	bool SetPresence(dpp::presence presence);
	bool ExecuteWebhook(dpp::webhook wh, const char* message, int allowed_mentions_mask, std::vector<dpp::snowflake> users, std::vector<dpp::snowflake> roles);
	bool ExecuteWebhookEmbed(dpp::webhook wh, const char* message, const DiscordEmbed* embed, int allowed_mentions_mask, std::vector<dpp::snowflake> users, std::vector<dpp::snowflake> roles);
	bool SendMessage(dpp::snowflake channel_id, const char* message, int allowed_mentions_mask, std::vector<dpp::snowflake> users, std::vector<dpp::snowflake> roles);
	bool SendMessageEmbed(dpp::snowflake channel_id, const char* message, const DiscordEmbed* embed, int allowed_mentions_mask, std::vector<dpp::snowflake> users, std::vector<dpp::snowflake> roles);
	bool SendDiscordMessage(const DiscordMessage* message);
	bool SendDiscordMessageToChannel(dpp::snowflake channel_id, const DiscordMessage* message);
	bool RegisterSlashCommandObject(dpp::snowflake guild_id, const dpp::slashcommand& command);
	bool RegisterGlobalSlashCommandObject(const dpp::slashcommand& command);
	void CreateAutocompleteResponse(dpp::snowflake id, const std::string &token, const dpp::interaction_response &response);
	bool EditMessage(dpp::snowflake channel_id, dpp::snowflake message_id, const char* content);
	bool EditMessageEmbed(dpp::snowflake channel_id, dpp::snowflake message_id, const char* content, const DiscordEmbed* embed);
	bool DeleteMessage(dpp::snowflake channel_id, dpp::snowflake message_id);
	bool DeleteMessageBulk(dpp::snowflake channel_id, const std::vector<dpp::snowflake>& message_ids);
	bool DeleteGuildCommand(dpp::snowflake guild_id, dpp::snowflake command_id);
	bool DeleteGlobalCommand(dpp::snowflake command_id);
	bool BulkDeleteGuildCommands(dpp::snowflake guild_id);
	bool BulkDeleteGlobalCommands();

	// Guild management methods
	bool LeaveGuild(dpp::snowflake guild_id);

	// Channel management methods
	bool CreateChannel(dpp::snowflake guild_id, const char* name, dpp::channel_type type, const char* topic = "", dpp::snowflake parent_id = 0, IForward* callback_forward = nullptr, cell_t data = 0);
	bool ModifyChannel(dpp::snowflake channel_id, const std::string& name = "", const std::string& topic = "", uint16_t position = 0, bool nsfw = false, uint16_t rate_limit = 0, uint16_t bitrate = 0, uint8_t user_limit = 0, dpp::snowflake parent_id = 0);
	bool DeleteChannel(dpp::snowflake channel_id);

	// Role management methods
	bool CreateRole(dpp::snowflake guild_id, const char* name, uint32_t color = 0, bool hoist = false, bool mentionable = false, uint64_t permissions = 0, IForward* callback_forward = nullptr, cell_t data = 0);
	bool CreateRoleFromObject(dpp::snowflake guild_id, const class DiscordRole* role_obj, IForward* callback_forward = nullptr, cell_t data = 0);
	bool ModifyRole(dpp::snowflake guild_id, dpp::snowflake role_id, const std::string& name = "", uint32_t color = 0, bool hoist = false, bool mentionable = false, uint64_t permissions = 0);
	bool DeleteRole(dpp::snowflake guild_id, dpp::snowflake role_id);

	// Member management methods
	bool ModifyMember(dpp::snowflake guild_id, dpp::snowflake user_id, const std::string& nickname = "");
	bool AddMemberRole(dpp::snowflake guild_id, dpp::snowflake user_id, dpp::snowflake role_id);
	bool RemoveMemberRole(dpp::snowflake guild_id, dpp::snowflake user_id, dpp::snowflake role_id);
	bool KickMember(dpp::snowflake guild_id, dpp::snowflake user_id);
	bool BanMember(dpp::snowflake guild_id, dpp::snowflake user_id, const char* reason = nullptr, int delete_message_days = 0);
	bool UnbanMember(dpp::snowflake guild_id, dpp::snowflake user_id);
	bool TimeoutMember(dpp::snowflake guild_id, dpp::snowflake user_id, time_t timeout_until);
	bool RemoveTimeout(dpp::snowflake guild_id, dpp::snowflake user_id);

	// Message management methods
	bool PinMessage(dpp::snowflake channel_id, dpp::snowflake message_id);
	bool UnpinMessage(dpp::snowflake channel_id, dpp::snowflake message_id);
	bool AddReaction(dpp::snowflake channel_id, dpp::snowflake message_id, const char* emoji);
	bool RemoveReaction(dpp::snowflake channel_id, dpp::snowflake message_id, const char* emoji);
	bool RemoveAllReactions(dpp::snowflake channel_id, dpp::snowflake message_id);

	// Webhook management methods
	bool ModifyWebhook(dpp::snowflake webhook_id, const std::string& name = "", const std::string& avatar_url = "");
	bool DeleteWebhook(dpp::snowflake webhook_id);

	std::string GetBotId() const { return m_cluster ? m_cluster->me.id.str() : ""; };
	const char* GetBotName() const { return m_cluster ? m_cluster->me.username.c_str() : ""; };
	uint16_t GetBotDiscriminator() const { return m_cluster ? m_cluster->me.discriminator : 0; };
	std::string GetBotAvatarUrl() const { return m_cluster ? m_cluster->me.get_avatar_url() : ""; };

	uint64_t GetUptime() const;
	int GetGuildCount() const;

	dpp::cluster* GetCluster() const { return m_cluster.get(); }
};

class DiscordInteraction
{
private:
	dpp::slashcommand_t m_interaction;
	std::string m_commandName;
	DiscordClient* m_client;
	mutable Handle_t m_userHandle;

public:
	DiscordInteraction(const dpp::slashcommand_t& interaction, DiscordClient* client) :
		m_interaction(interaction),
		m_commandName(interaction.command.get_command_name()),
		m_client(client),
		m_userHandle(BAD_HANDLE)
	{
	}

	~DiscordInteraction() {
		if (m_userHandle) {
			handlesys->FreeHandle(m_userHandle, nullptr);
		}
	}

	const char* GetCommandName() const { return m_commandName.c_str(); }
	std::string GetGuildId() const { return m_interaction.command.guild_id.str(); }
	std::string GetChannelId() const { return m_interaction.command.channel_id.str(); }

	Handle_t GetUserHandle() const;

	DiscordUser* GetUser() const { 
		if (m_interaction.command.guild_id != 0 && m_interaction.command.member.user_id != 0) {
			return new DiscordUser(m_interaction.command.usr, m_interaction.command.member, m_client);
		}
		return new DiscordUser(m_interaction.command.usr, m_client); 
	}
	std::string GetUserId() const { return m_interaction.command.usr.id.str(); }
	const char* GetUserName() const { return m_interaction.command.usr.username.c_str(); }
	std::string GetUserNickname() const { return m_interaction.command.member.get_nickname(); }

	bool GetOptionValue(const char* name, std::string& value) const {
		auto param = m_interaction.get_parameter(name);
		if (param.index() == 0) return false;
		value = std::get<std::string>(param);
		return true;
	}

	bool GetOptionValueInt(const char* name, int64_t& value) const {
		auto param = m_interaction.get_parameter(name);
		if (param.index() == 0) return false;
		value = std::get<int64_t>(param);
		return true;
	}

	bool GetOptionValueDouble(const char* name, double& value) const {
		auto param = m_interaction.get_parameter(name);
		if (param.index() == 0) return false;
		value = std::get<double>(param);
		return true;
	}

	bool GetOptionValueBool(const char* name, bool& value) const {
		auto param = m_interaction.get_parameter(name);
		if (param.index() == 0) return false;
		value = std::get<bool>(param);
		return true;
	}

	void CreateResponse(const char* content) const {
		m_interaction.reply(dpp::message(content));
	}

	void CreateResponseEmbed(const char* content, const DiscordEmbed* embed) const {
		dpp::message msg(content);
		msg.add_embed(embed->GetEmbed());
		m_interaction.reply(msg);
	}

	void DeferReply(bool ephemeral = false) const {
		m_interaction.thinking(ephemeral);
	}

	void EditResponse(const char* content) const {
		m_interaction.edit_response(dpp::message(content));
	}

	void CreateEphemeralResponse(const char* content) const {
		dpp::message msg(content);
		msg.set_flags(dpp::m_ephemeral);
		m_interaction.reply(msg);
	}

	void CreateEphemeralResponseEmbed(const char* content, const DiscordEmbed* embed) const {
		dpp::message msg(content);
		msg.set_flags(dpp::m_ephemeral);
		msg.add_embed(embed->GetEmbed());
		m_interaction.reply(msg);
	}
};

class DiscordAutocompleteInteraction
{
private:
	mutable Handle_t m_userHandle;

public:
	dpp::interaction_response m_response;
	std::string m_commandName;
	dpp::interaction m_command;
	dpp::autocomplete_t m_autocomplete;
	DiscordClient* m_client;

	DiscordAutocompleteInteraction(const dpp::autocomplete_t& autocomplete, DiscordClient* client) :
		m_userHandle(BAD_HANDLE),
		m_response(dpp::ir_autocomplete_reply),
		m_commandName(autocomplete.command.get_command_name()),
		m_command(autocomplete.command),
		m_autocomplete(autocomplete),
		m_client(client)
	{
	}

	~DiscordAutocompleteInteraction() {
		if (m_userHandle) {
			handlesys->FreeHandle(m_userHandle, nullptr);
		}
	}

	const char* GetCommandName() const { return m_commandName.c_str(); }
	std::string GetGuildId() const { return m_command.guild_id.str(); }
	std::string GetChannelId() const { return m_command.channel_id.str(); }

	Handle_t GetUserHandle() const;

	DiscordUser* GetUser() const { 
		if (m_command.guild_id != 0 && m_command.member.user_id != 0) {
			return new DiscordUser(m_command.usr, m_command.member, m_client);
		}
		return new DiscordUser(m_command.usr, m_client); 
	}
	std::string GetUserNickname() const { return m_command.member.get_nickname(); }

	dpp::command_option GetOption(const char* name) const {
		for (auto & opt : m_autocomplete.options) {
			if (opt.name == name) return opt;
		}

		throw std::runtime_error("Option not found");
	}

	std::string GetOptionValue(const char* name) const {
		return std::get<std::string>(GetOption(name).value);
	}

	int64_t GetOptionValueInt(const char* name) const {
		return std::get<int64_t>(GetOption(name).value);
	}

	double GetOptionValueDouble(const char* name) const {
		return std::get<double>(GetOption(name).value);
	}

	bool GetOptionValueBool(const char* name) const {
		return std::get<bool>(GetOption(name).value);
	}

	void AddAutocompleteOption(dpp::command_option_choice choice) {
		m_response.add_autocomplete_choice(choice);
	}
};

class DiscordGuild
{
private:
	dpp::guild m_guild;
	DiscordClient* m_client;

public:
	DiscordGuild(const dpp::guild& guild) : m_guild(guild), m_client(nullptr) {}
	DiscordGuild(const dpp::guild& guild, DiscordClient* client) : m_guild(guild), m_client(client) {}

	// Basic guild information
	std::string GetId() const { return m_guild.id.str(); }
	const char* GetName() const { return m_guild.name.c_str(); }
	const char* GetDescription() const { return m_guild.description.c_str(); }
	std::string GetOwnerId() const { return m_guild.owner_id.str(); }
	std::string GetApplicationId() const { return m_guild.application_id.str(); }

	// Guild URLs and icons
	std::string GetIconUrl(uint16_t size = 0, bool prefer_animated = true) const { 
		return m_guild.get_icon_url(size, dpp::i_png, prefer_animated); 
	}
	std::string GetBannerUrl(uint16_t size = 0, bool prefer_animated = true) const { 
		return m_guild.get_banner_url(size, dpp::i_png, prefer_animated); 
	}
	std::string GetSplashUrl(uint16_t size = 0) const { 
		return m_guild.get_splash_url(size); 
	}
	std::string GetDiscoverySplashUrl(uint16_t size = 0) const { 
		return m_guild.get_discovery_splash_url(size); 
	}

	// Guild features and flags
	bool IsLarge() const { return m_guild.is_large(); }
	bool IsUnavailable() const { return m_guild.is_unavailable(); }
	bool WidgetEnabled() const { return m_guild.widget_enabled(); }
	bool HasInviteSplash() const { return m_guild.has_invite_splash(); }
	bool HasVipRegions() const { return m_guild.has_vip_regions(); }
	bool HasVanityUrl() const { return m_guild.has_vanity_url(); }
	bool IsVerified() const { return m_guild.is_verified(); }
	bool IsPartnered() const { return m_guild.is_partnered(); }
	bool IsCommunity() const { return m_guild.is_community(); }
	bool HasRoleSubscriptions() const { return m_guild.has_role_subscriptions(); }
	bool HasNews() const { return m_guild.has_news(); }
	bool IsDiscoverable() const { return m_guild.is_discoverable(); }
	bool IsFeatureable() const { return m_guild.is_featureable(); }
	bool HasAnimatedIcon() const { return m_guild.has_animated_icon(); }
	bool HasBanner() const { return m_guild.has_banner(); }
	bool IsWelcomeScreenEnabled() const { return m_guild.is_welcome_screen_enabled(); }
	bool HasMemberVerificationGate() const { return m_guild.has_member_verification_gate(); }
	bool IsPreviewEnabled() const { return m_guild.is_preview_enabled(); }
	bool HasAnimatedIconHash() const { return m_guild.has_animated_icon_hash(); }
	bool HasAnimatedBannerHash() const { return m_guild.has_animated_banner_hash(); }
	bool HasMonetizationEnabled() const { return m_guild.has_monetization_enabled(); }
	bool HasMoreStickers() const { return m_guild.has_more_stickers(); }
	bool HasCreatorStorePage() const { return m_guild.has_creator_store_page(); }
	bool HasRoleIcons() const { return m_guild.has_role_icons(); }
	bool HasTicketedEvents() const { return m_guild.has_ticketed_events(); }
	bool HasPremiumProgressBarEnabled() const { return m_guild.has_premium_progress_bar_enabled(); }
	bool HasInvitesDisabled() const { return m_guild.has_invites_disabled(); }
	bool HasAnimatedBanner() const { return m_guild.has_animated_banner(); }
	bool HasAutoModeration() const { return m_guild.has_auto_moderation(); }
	bool HasSupportServer() const { return m_guild.has_support_server(); }
	bool HasRoleSubscriptionsAvailableForPurchase() const { return m_guild.has_role_subscriptions_available_for_purchase(); }
	bool HasRaidAlertsDisabled() const { return m_guild.has_raid_alerts_disabled(); }

	// Guild properties
	uint64_t GetMemberCount() const { return dpp::get_user_count(); }
	uint32_t GetMaxPresences() const { return m_guild.max_presences; }
	uint32_t GetMaxMembers() const { return m_guild.max_members; }
	uint16_t GetPremiumSubscriptionCount() const { return m_guild.premium_subscription_count; }
	uint8_t GetMaxVideoChannelUsers() const { return m_guild.max_video_channel_users; }
	uint8_t GetPremiumTier() const { return static_cast<uint8_t>(m_guild.premium_tier); }
	uint8_t GetVerificationLevel() const { return static_cast<uint8_t>(m_guild.verification_level); }
	uint8_t GetExplicitContentFilter() const { return static_cast<uint8_t>(m_guild.explicit_content_filter); }
	uint8_t GetMfaLevel() const { return static_cast<uint8_t>(m_guild.mfa_level); }
	uint8_t GetNsfwLevel() const { return static_cast<uint8_t>(m_guild.nsfw_level); }
	uint8_t GetAfkTimeout() const { return static_cast<uint8_t>(m_guild.afk_timeout); }
	uint8_t GetDefaultMessageNotifications() const { return static_cast<uint8_t>(m_guild.default_message_notifications); }

	// Channel IDs
	std::string GetAfkChannelId() const { return m_guild.afk_channel_id.str(); }
	std::string GetSystemChannelId() const { return m_guild.system_channel_id.str(); }
	std::string GetRulesChannelId() const { return m_guild.rules_channel_id.str(); }
	std::string GetPublicUpdatesChannelId() const { return m_guild.public_updates_channel_id.str(); }
	std::string GetWidgetChannelId() const { return m_guild.widget_channel_id.str(); }
	std::string GetSafetyAlertsChannelId() const { return m_guild.safety_alerts_channel_id.str(); }

	// Collections
	uint64_t GetRoleCount() const { return dpp::get_role_count(); }
	uint64_t GetEmojiCount() const { return dpp::get_emoji_count(); }
	uint64_t GetChannelCount() const { return dpp::get_channel_count(); }
	size_t GetVoiceMemberCount() const { return m_guild.voice_members.size(); }
	size_t GetThreadCount() const { return m_guild.threads.size(); }

	// Collection accessors
	std::string GetRoleId(size_t index) const {
		if (index >= m_guild.roles.size()) return "";
		return m_guild.roles[index].str();
	}
	std::string GetChannelId(size_t index) const {
		if (index >= m_guild.channels.size()) return "";
		return m_guild.channels[index].str();
	}
	std::string GetThreadId(size_t index) const {
		if (index >= m_guild.threads.size()) return "";
		return m_guild.threads[index].str();
	}
	std::string GetEmojiId(size_t index) const {
		if (index >= m_guild.emojis.size()) return "";
		return m_guild.emojis[index].str();
	}

	// Additional properties
	const char* GetVanityUrlCode() const { return m_guild.vanity_url_code.c_str(); }
	uint32_t GetFlags() const { return m_guild.flags; }
	uint16_t GetFlagsExtra() const { return m_guild.flags_extra; }
	uint16_t GetShardId() const { return m_guild.shard_id; }

	// Welcome screen
	bool HasWelcomeScreen() const { return !m_guild.welcome_screen.description.empty(); }
	const char* GetWelcomeScreenDescription() const { return m_guild.welcome_screen.description.c_str(); }
	size_t GetWelcomeChannelCount() const { return m_guild.welcome_screen.welcome_channels.size(); }
	std::string GetWelcomeChannelId(size_t index) const {
		if (index >= m_guild.welcome_screen.welcome_channels.size()) return "";
		return m_guild.welcome_screen.welcome_channels[index].channel_id.str();
	}
	const char* GetWelcomeChannelDescription(size_t index) const {
		if (index >= m_guild.welcome_screen.welcome_channels.size()) return "";
		return m_guild.welcome_screen.welcome_channels[index].description.c_str();
	}

	// Additional flag checking methods that were missing
	bool HasSevenDayThreadArchive() const { return m_guild.has_seven_day_thread_archive(); }
	bool HasThreeDayThreadArchive() const { return m_guild.has_three_day_thread_archive(); }
	bool HasChannelBanners() const { return m_guild.has_channel_banners(); }

	// Additional notification settings - these check for disabled notifications
	bool NoJoinNotifications() const { return (m_guild.flags & dpp::g_no_join_notifications) != 0; }
	bool NoBoostNotifications() const { return (m_guild.flags & dpp::g_no_boost_notifications) != 0; }
	bool NoSetupTips() const { return (m_guild.flags & dpp::g_no_setup_tips) != 0; }
	bool NoStickerGreeting() const { return (m_guild.flags & dpp::g_no_sticker_greeting) != 0; }
	bool NoRoleSubscriptionNotifications() const { return (m_guild.flags_extra & dpp::g_no_role_subscription_notifications) != 0; }
	bool NoRoleSubscriptionNotificationReplies() const { return (m_guild.flags_extra & dpp::g_no_role_subscription_notification_replies) != 0; }

	// Permission methods for users
	uint64_t GetBasePermissions(dpp::snowflake user_id) const;
	uint64_t GetPermissionsInChannel(dpp::snowflake user_id, dpp::snowflake channel_id) const;
	bool HasPermission(dpp::snowflake user_id, const char* permission) const;
	bool HasPermissionInChannel(dpp::snowflake user_id, dpp::snowflake channel_id, const char* permission) const;

	// Guild management methods
	bool Modify(const char* name = nullptr, const char* description = nullptr);
	bool GetInvites(IForward* callback_forward, cell_t data = 0);
	bool GetWebhooks(IForward* callback_forward, cell_t data = 0);

	// Internal accessor
	const dpp::guild& GetDPPGuild() const { return m_guild; }
};

class DiscordRole
{
private:
	dpp::role m_role;
	dpp::snowflake m_guild_id;
	DiscordClient* m_client;

public:
	DiscordRole(DiscordClient* client) : m_guild_id(0), m_client(client) {} // Empty constructor with client binding
	DiscordRole(const dpp::role& role, dpp::snowflake guild_id) : m_role(role), m_guild_id(guild_id), m_client(nullptr) {}
	DiscordRole(const dpp::role& role, dpp::snowflake guild_id, DiscordClient* client) : m_role(role), m_guild_id(guild_id), m_client(client) {}

	// Basic role information
	std::string GetId() const { return m_role.id.str(); }
	const char* GetName() const { return m_role.name.c_str(); }
	uint32_t GetColor() const { return m_role.colour; }
	bool IsHoisted() const { return m_role.is_hoisted(); }
	int32_t GetPosition() const { return m_role.position; }
	uint64_t GetPermissions() const { return static_cast<uint64_t>(m_role.permissions); }
	bool IsManaged() const { return m_role.is_managed(); }
	bool IsMentionable() const { return m_role.is_mentionable(); }

	// Icon
	std::string GetIconHash() const { 
#ifdef _WIN32
		// Disabled on Windows due to linking issues (Fix it later)
		return "";
#else
		if (m_role.icon.is_iconhash()) {
			return m_role.icon.as_iconhash().to_string();
		}
		return "";
#endif
	}
	const char* GetUnicodeEmoji() const { return m_role.unicode_emoji.c_str(); }

	// Tags (stored in flags)
	bool HasTags() const { 
		return (m_role.flags & (dpp::r_premium_subscriber | dpp::r_available_for_purchase | dpp::r_guild_connections)) != 0
			|| m_role.bot_id != 0 
			|| m_role.integration_id != 0 
			|| m_role.subscription_listing_id != 0;
	}
	bool IsPremiumSubscriberRole() const { 
		return m_role.flags & dpp::r_premium_subscriber; 
	}
	bool IsAvailableForPurchase() const { 
		return m_role.flags & dpp::r_available_for_purchase; 
	}
	bool IsGuildConnections() const { 
		return m_role.flags & dpp::r_guild_connections; 
	}

	// Tag IDs
	std::string GetBotId() const {
		if (m_role.bot_id != 0) {
			return m_role.bot_id.str();
		}
		return "";
	}
	std::string GetIntegrationId() const {
		if (m_role.integration_id != 0) {
			return m_role.integration_id.str();
		}
		return "";
	}
	std::string GetSubscriptionListingId() const {
		if (m_role.subscription_listing_id != 0) {
			return m_role.subscription_listing_id.str();
		}
		return "";
	}

	// Permission methods
	bool HasPermission(const char* permission) const;
	std::string GetMention() const { return m_role.get_mention(); }

	// Role management methods
	bool Modify(const char* name = nullptr, int color = -1, int hoist = -1, int mentionable = -1);
	bool Delete();
	bool AddToUser(dpp::snowflake user_id);
	bool RemoveFromUser(dpp::snowflake user_id);

	// Setters for local role properties (for building new roles)
	void SetName(const char* name) { if (name) m_role.name = name; }
	void SetColor(uint32_t color) { m_role.colour = color; }
	void SetHoist(bool hoist) { 
		if (hoist) m_role.flags |= dpp::r_hoist;
		else m_role.flags &= ~dpp::r_hoist;
	}
	void SetMentionable(bool mentionable) {
		if (mentionable) m_role.flags |= dpp::r_mentionable;
		else m_role.flags &= ~dpp::r_mentionable;
	}
	void SetPermissions(uint64_t permissions) { m_role.permissions = permissions; }

	// Internal accessor
	const dpp::role& GetDPPRole() const { return m_role; }
	dpp::snowflake GetGuildId() const { return m_guild_id; }
};

class DiscordInvite
{
private:
	dpp::invite m_invite;
	DiscordClient* m_client;

public:
	DiscordInvite(const dpp::invite& invite, DiscordClient* client = nullptr) : m_invite(invite), m_client(client) {}
	DiscordInvite(DiscordClient* client) : m_client(client) {}

	// Basic properties
	const char* GetCode() const { return m_invite.code.c_str(); }
	std::string GetGuildId() const { return m_invite.guild_id.str(); }
	std::string GetChannelId() const { return m_invite.channel_id.str(); }
	std::string GetInviterId() const { return m_invite.inviter.id.str(); }
	std::string GetTargetUserId() const { return m_invite.target_user_id.str(); }

	// Inviter information
	const char* GetInviterUsername() const { return m_invite.inviter.username.c_str(); }
	std::string GetInviterAvatarUrl() const { return m_invite.inviter.get_avatar_url(); }
	bool GetInviterIsBot() const { return m_invite.inviter.is_bot(); }

	// Guild/Channel names (from partial objects)
	const char* GetGuildName() const { return m_invite.destination_guild.name.c_str(); }
	const char* GetChannelName() const { return m_invite.destination_channel.name.c_str(); }
	uint8_t GetChannelType() const { return static_cast<uint8_t>(m_invite.destination_channel.get_type()); }

	// Invite statistics
	uint32_t GetApproximatePresenceCount() const { return m_invite.approximate_presence_count; }
	uint32_t GetApproximateMemberCount() const { return m_invite.approximate_member_count; }
	uint32_t GetUses() const { return m_invite.uses; }
	uint32_t GetMaxUses() const { return m_invite.max_uses; }
	uint32_t GetMaxAge() const { return m_invite.max_age; }

	// Invite properties
	uint8_t GetTargetType() const { return static_cast<uint8_t>(m_invite.target_type); }
	bool IsTemporary() const { return m_invite.temporary; }
	bool IsUnique() const { return m_invite.unique; }
	time_t GetCreatedAt() const { return m_invite.created_at; }
	time_t GetExpiresAt() const { return m_invite.expires_at; }

	// Setters for creating invites
	void SetMaxAge(uint32_t max_age) { m_invite.set_max_age(max_age); }
	void SetMaxUses(uint8_t max_uses) { m_invite.set_max_uses(max_uses); }
	void SetTargetUserId(dpp::snowflake user_id) { m_invite.set_target_user_id(user_id); }
	void SetTargetType(uint8_t type) { m_invite.set_target_type(static_cast<dpp::invite_target_t>(type)); }
	void SetTemporary(bool temporary) { m_invite.set_temporary(temporary); }
	void SetUnique(bool unique) { m_invite.set_unique(unique); }

	// Invite management
	bool Create(dpp::snowflake channel_id);
	bool Delete();

	// Internal accessor
	const dpp::invite& GetDPPInvite() const { return m_invite; }
};

class DiscordSlashCommand
{
private:
	dpp::slashcommand m_command;
	std::vector<dpp::command_option> m_options;
	DiscordClient* m_client;
	dpp::snowflake m_guild_id; // Store guild ID for guild-specific commands
	std::vector<dpp::command_permission> m_permissions; // Store command permissions

public:
	DiscordSlashCommand(DiscordClient* client) : m_client(client), m_guild_id(0) {}

	void SetName(const char* name) { m_command.set_name(name); }
	void SetDescription(const char* description) { m_command.set_description(description); }
	void SetDefaultPermissions(const char* permissions) {
		if (permissions && permissions[0] != '\0') {
			m_command.set_default_permissions(std::stoull(permissions));
		}
	}
	void SetApplicationId(dpp::snowflake app_id) { m_command.set_application_id(app_id); }

	// Methods for managing existing commands
	void SetCommandId(dpp::snowflake command_id) { m_command.id = command_id; }
	void SetGuildId(dpp::snowflake guild_id) { m_guild_id = guild_id; }
	dpp::snowflake GetCommandId() const { return m_command.id; }
	dpp::snowflake GetGuildId() const { return m_guild_id; }

	const char* GetName() const { return m_command.name.c_str(); }
	const char* GetDescription() const { return m_command.description.c_str(); }
	std::string GetDefaultPermissions() const { 
		char permStr[32];
		snprintf(permStr, sizeof(permStr), "%" PRIu64, static_cast<uint64_t>(m_command.default_member_permissions));
		return std::string(permStr);
	}

	void AddOption(const char* name, const char* description, dpp::command_option_type type, bool required = false, bool autocomplete = false) {
		dpp::command_option option(type, name, description, required);
		option.set_auto_complete(autocomplete);
		m_options.push_back(option);
		m_command.options = m_options;
	}

	void AddChoiceOption(const char* name, const char* description, dpp::command_option_type type, bool required = false) {
		dpp::command_option option(type, name, description, required);
		m_options.push_back(option);
		m_command.options = m_options;
	}

	void AddStringChoice(const char* choice_name, const char* choice_value) {
		if (!m_options.empty()) {
			m_options.back().add_choice(dpp::command_option_choice(choice_name, std::string(choice_value)));
			m_command.options = m_options;
		}
	}

	void AddIntChoice(const char* choice_name, int64_t choice_value) {
		if (!m_options.empty()) {
			m_options.back().add_choice(dpp::command_option_choice(choice_name, choice_value));
			m_command.options = m_options;
		}
	}

	void AddFloatChoice(const char* choice_name, double choice_value) {
		if (!m_options.empty()) {
			m_options.back().add_choice(dpp::command_option_choice(choice_name, choice_value));
			m_command.options = m_options;
		}
	}

	bool RegisterToGuild(dpp::snowflake guild_id) {
		if (!m_client) return false;
		m_command.set_application_id(m_client->GetCluster()->me.id);
		return m_client->RegisterSlashCommandObject(guild_id, m_command);
	}

	bool RegisterGlobally() {
		if (!m_client) return false;
		m_command.set_application_id(m_client->GetCluster()->me.id);
		return m_client->RegisterGlobalSlashCommandObject(m_command);
	}

	// New advanced functionality methods
	void SetContextMenuType(dpp::slashcommand_contextmenu_type type) { m_command.set_type(type); }
	dpp::slashcommand_contextmenu_type GetContextMenuType() const { return m_command.type; }

	void SetNSFW(bool nsfw) { m_command.set_nsfw(nsfw); }
	bool GetNSFW() const { return m_command.nsfw; }

	void SetDMPermission(bool dm_permission) { m_command.set_dm_permission(dm_permission); }
	bool GetDMPermission() const { return m_command.dm_permission; }

	void AddLocalization(const char* language, const char* name, const char* description = nullptr) {
		if (description && strlen(description) > 0) {
			m_command.add_localization(language, name, description);
		} else {
			m_command.add_localization(language, name);
		}
	}

	void SetInteractionContexts(const std::vector<dpp::interaction_context_type>& contexts) {
		m_command.set_interaction_contexts(contexts);
	}

	void SetIntegrationTypes(const std::vector<dpp::application_integration_types>& types) {
		m_command.integration_types.clear();
		for (const auto& type : types) {
			m_command.integration_types.push_back(type);
		}
	}

	// Option-specific methods for the last added option
	void SetLastOptionMinValue(double min_value) {
		if (!m_options.empty()) {
			m_options.back().set_min_value(min_value);
			m_command.options = m_options;
		}
	}

	void SetLastOptionMaxValue(double max_value) {
		if (!m_options.empty()) {
			m_options.back().set_max_value(max_value);
			m_command.options = m_options;
		}
	}

	void SetLastOptionMinLength(int64_t min_length) {
		if (!m_options.empty()) {
			m_options.back().set_min_length(min_length);
			m_command.options = m_options;
		}
	}

	void SetLastOptionMaxLength(int64_t max_length) {
		if (!m_options.empty()) {
			m_options.back().set_max_length(max_length);
			m_command.options = m_options;
		}
	}

	void AddLastOptionChannelType(dpp::channel_type channel_type) {
		if (!m_options.empty()) {
			m_options.back().add_channel_type(channel_type);
			m_command.options = m_options;
		}
	}

	std::string GetMention() const { return m_command.get_mention(); }

	// Permission override management methods
	void AddPermissionOverride(dpp::snowflake target_id, dpp::command_permission_type type, bool permission) {
		dpp::command_permission perm;
		perm.id = target_id;
		perm.type = type;
		perm.permission = permission;

		RemovePermissionOverride(target_id, type);
		m_permissions.push_back(perm);
	}

	void RemovePermissionOverride(dpp::snowflake target_id, dpp::command_permission_type type) {
		m_permissions.erase(
			std::remove_if(m_permissions.begin(), m_permissions.end(),
				[target_id, type](const dpp::command_permission& perm) {
					return perm.id == target_id && perm.type == type;
				}),
			m_permissions.end()
		);
	}

	void ClearPermissionOverrides() { m_permissions.clear(); }

	size_t GetPermissionOverrideCount() const { return m_permissions.size(); }

	bool GetPermissionOverride(size_t index, dpp::snowflake& target_id, dpp::command_permission_type& type, bool& permission) const {
		if (index >= m_permissions.size()) return false;
		target_id = m_permissions[index].id;
		type = m_permissions[index].type;
		permission = m_permissions[index].permission;
		return true;
	}


	bool Update(dpp::snowflake guild_id = 0);
	bool Delete(dpp::snowflake guild_id = 0);
	bool ApplyPermissionOverrides(dpp::snowflake guild_id);

	const dpp::slashcommand& GetCommand() const { return m_command; }
};

// HTTP Classes
class HttpHeaders
{
private:
	std::map<std::string, std::string> m_headers;

public:
	HttpHeaders() {}

	void SetHeader(const char* name, const char* value) {
		m_headers[name] = value;
	}

	bool GetHeader(const char* name, std::string& value) const {
		auto it = m_headers.find(name);
		if (it != m_headers.end()) {
			value = it->second;
			return true;
		}
		return false;
	}

	bool RemoveHeader(const char* name) {
		return m_headers.erase(name) > 0;
	}

	void ClearHeaders() {
		m_headers.clear();
	}

	size_t Count() const {
		return m_headers.size();
	}

	bool GetHeaderByIndex(size_t index, std::string& name, std::string& value) const {
		if (index >= m_headers.size()) return false;
		auto it = m_headers.begin();
		std::advance(it, index);
		name = it->first;
		value = it->second;
		return true;
	}

	// Convert to DPP headers format
	dpp::http_headers ToDppHeaders() const {
		dpp::http_headers headers;
		for (const auto& pair : m_headers) {
			headers.emplace(pair.first, pair.second);
		}
		return headers;
	}
};

class HttpCompletion
{
private:
	dpp::http_request_completion_t m_completion;
	std::string m_protocol;
	mutable Handle_t m_responseHeadersHandle;

public:
	HttpCompletion(const dpp::http_request_completion_t& completion, const std::string& protocol = "1.1") 
		: m_completion(completion), m_protocol(protocol), m_responseHeadersHandle(BAD_HANDLE) {}

	~HttpCompletion() {
		if (m_responseHeadersHandle) {
			handlesys->FreeHandle(m_responseHeadersHandle, nullptr);
		}
	}

	// Store the handle when ResponseHeaders is accessed
	void SetResponseHeadersHandle(Handle_t handle) const {
		m_responseHeadersHandle = handle;
	}

	// Get the stored handle
	Handle_t GetResponseHeadersHandle() const {
		return m_responseHeadersHandle;
	}

	int GetStatus() const {
		return static_cast<int>(m_completion.status);
	}

	const std::string& GetBody() const {
		return m_completion.body;
	}

	size_t GetBodyLength() const {
		return m_completion.body.length();
	}

	// Create HttpHeaders object from response headers
	HttpHeaders* CreateResponseHeaders() const {
		HttpHeaders* headers = new HttpHeaders();
		for (const auto& pair : m_completion.headers) {
			headers->SetHeader(pair.first.c_str(), pair.second.c_str());
		}
		return headers;
	}

	const std::string& GetProtocol() const {
		return m_protocol;
	}

	bool IsSuccess() const {
		return m_completion.error == dpp::http_error::h_success && m_completion.status >= 200 && m_completion.status < 300;
	}
};

#endif // _INCLUDE_DISCORD_H_ 