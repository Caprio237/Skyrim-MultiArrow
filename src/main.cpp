// MultiArrowCharge - milestone 1
// Only logs bow-related animation events so we can confirm the plugin loads
// on Skyrim 1.7.104 and learn the exact event names the game sends.

#include "RE/Skyrim.h"
#include "SKSE/SKSE.h"

#include <spdlog/sinks/basic_file_sink.h>

#include <algorithm>
#include <cctype>
#include <memory>
#include <string>

namespace logger = SKSE::log;

namespace
{
    void SetupLog()
    {
        auto path = logger::log_directory();
        if (!path) {
            return;
        }
        *path /= "MultiArrowCharge.log";

        auto sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true);
        auto log = std::make_shared<spdlog::logger>(std::string("global log"), std::move(sink));
        log->set_level(spdlog::level::info);
        log->flush_on(spdlog::level::info);
        spdlog::set_default_logger(std::move(log));
        spdlog::set_pattern("[%H:%M:%S.%e] [%l] %v");
    }

    bool LooksBowRelated(std::string a_tag)
    {
        std::transform(a_tag.begin(), a_tag.end(), a_tag.begin(),
            [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
        return a_tag.find("bow") != std::string::npos || a_tag.find("arrow") != std::string::npos;
    }

    class DrawWatcher : public RE::BSTEventSink<RE::BSAnimationGraphEvent>
    {
    public:
        static DrawWatcher* GetSingleton()
        {
            static DrawWatcher singleton;
            return std::addressof(singleton);
        }

        RE::BSEventNotifyControl ProcessEvent(
            const RE::BSAnimationGraphEvent* a_event,
            RE::BSTEventSource<RE::BSAnimationGraphEvent>*) override
        {
            if (a_event) {
                const char* tag = a_event->tag.c_str();
                if (tag && LooksBowRelated(tag)) {
                    logger::info("anim event: {}", tag);
                }
            }
            return RE::BSEventNotifyControl::kContinue;
        }
    };

    void AttachWatcher()
    {
        auto player = RE::PlayerCharacter::GetSingleton();
        if (!player) {
            logger::warn("player not available yet");
            return;
        }
        player->RemoveAnimationGraphEventSink(DrawWatcher::GetSingleton());
        const bool ok = player->AddAnimationGraphEventSink(DrawWatcher::GetSingleton());
        logger::info("attached animation watcher: {}", ok);
    }

    void OnMessage(SKSE::MessagingInterface::Message* a_msg)
    {
        switch (a_msg->type) {
        case SKSE::MessagingInterface::kPostLoadGame:
        case SKSE::MessagingInterface::kNewGame:
            AttachWatcher();
            break;
        default:
            break;
        }
    }
}

SKSEPluginLoad(const SKSE::LoadInterface* a_skse)
{
    SKSE::Init(a_skse, false);
    SetupLog();
    logger::info("MultiArrowCharge milestone 1 loaded");
    SKSE::GetMessagingInterface()->RegisterListener(OnMessage);
    return true;
}
