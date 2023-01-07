#include "UI/QosmeticsFlowCoordinator.hpp"
#include "Data/Creators.hpp"
#include "UI/CreditViewController.hpp"
#include "UI/ProfileSwitcherViewController.hpp"
#include "UI/QosmeticsViewController.hpp"
#include "Utils/DateUtils.hpp"
#include "Utils/RainbowUtils.hpp"
#include "Utils/UIUtils.hpp"
#include "logging.hpp"
#include "static-defines.hpp"

#include "HMUI/ButtonSpriteSwap.hpp"
#include "HMUI/TitleViewController.hpp"
#include "HMUI/ViewController_AnimationDirection.hpp"
#include "HMUI/ViewController_AnimationType.hpp"
#include "Polyglot/LocalizedTextMeshProUGUI.hpp"
#include "UnityEngine/Events/UnityAction.hpp"
#include "UnityEngine/TextureWrapMode.hpp"
#include "UnityEngine/UI/Button_ButtonClickedEvent.hpp"
#include "UnityEngine/UI/HorizontalLayoutGroup.hpp"

#include "bsml/shared/BSML.hpp"
#include "bsml/shared/BSML/Settings/BSMLSettings.hpp"
#include "bsml/shared/Helpers/utilities.hpp"

#include "TMPro/TMP_TextInfo.hpp"
#include "TMPro/TMP_VertexDataUpdateFlags.hpp"

#include "assets.hpp"
DEFINE_TYPE(Qosmetics::Core, QosmeticsFlowCoordinator);

using namespace UnityEngine;
using namespace UnityEngine::UI;

namespace Qosmetics::Core
{
    void QosmeticsFlowCoordinator::ctor()
    {
        INVOKE_BASE_CTOR(classof(HMUI::FlowCoordinator*));
    }

    void QosmeticsFlowCoordinator::Inject(Qosmetics::Core::CreditViewController* creditViewController, Qosmetics::Core::ProfileSwitcherViewController* profileSwitcherViewController, Qosmetics::Core::QosmeticsViewController* qosmeticsViewController, GlobalNamespace::MainFlowCoordinator* mainFlowCoordinator)
    {
        this->creditViewController = creditViewController;
        this->profileSwitcherViewController = profileSwitcherViewController;
        this->qosmeticsViewController = qosmeticsViewController;
        this->mainFlowCoordinator = mainFlowCoordinator;
        qosmeticsViewController->set_qosmeticsFlowCoordinator(this);
    }

    void QosmeticsFlowCoordinator::Initialize()
    {
        Qosmetics::Core::Creators::Download();

        auto optionsViewController = mainFlowCoordinator->optionsViewController;
        bool bsmlSettingsExists = BSML::BSMLSettings::get_instance()->get_settingsMenus().size() > 0;
        vanillaMenus = List<UnityEngine::Transform*>::New_ctor();
        UnityEngine::Transform* wrapper = optionsViewController->get_transform()->Find("Wrapper");

        // load sprites for btn
        activeSprite = BSML::Utilities::LoadSpriteRaw(IncludedAssets::MenuIconSelected_png);
        inactiveSprite = BSML::Utilities::LoadSpriteRaw(IncludedAssets::MenuIcon_png);

        activeSprite->get_texture()->set_wrapMode(UnityEngine::TextureWrapMode::Clamp);
        inactiveSprite->get_texture()->set_wrapMode(UnityEngine::TextureWrapMode::Clamp);

        // make button
        BSML::parse_and_construct("<image-button id='settingsButton' text='Qosmetics Settings' on-click='PresentSelf'/>", wrapper, this);

        if (bsmlSettingsExists)
        {
            vanillaMenus->Add(optionsViewController->editAvatarButton->get_transform());
            vanillaMenus->Add(optionsViewController->playerOptionsButton->get_transform());
            vanillaMenus->Add(optionsViewController->settingsButton->get_transform());
            BSML::parse_and_construct(IncludedAssets::QosmeticsSettingsButton_bsml, optionsViewController->get_transform(), this);

            // this has to be done manually, without bsml
            HorizontalLayoutGroup* oldLayout = wrapper->get_gameObject()->GetComponent<HorizontalLayoutGroup*>();
            oldLayout->get_gameObject()->GetComponent<RectTransform*>()->set_anchoredPosition(UnityEngine::Vector2(0.0f, 0.0f));
            oldLayout->set_spacing(2);
            settingsButton->get_transform()->SetAsLastSibling();
        }
        else
            settingsButton->get_transform()->SetAsFirstSibling();
    }

    void QosmeticsFlowCoordinator::PostParse()
    {
        if (!(settingsButton && settingsButton->m_CachedPtr.m_value))
            return;

        auto spriteSwap = settingsButton->GetComponent<HMUI::ButtonSpriteSwap*>();
        // setting the sprites
        spriteSwap->highlightStateSprite = spriteSwap->pressedStateSprite = activeSprite;
        spriteSwap->disabledStateSprite = spriteSwap->normalStateSprite = inactiveSprite;
    }

    void QosmeticsFlowCoordinator::PresentSelf()
    {
        if (!mainFlowCoordinator || !mainFlowCoordinator->m_CachedPtr.m_value)
            return;
        mainFlowCoordinator->YoungestChildFlowCoordinatorOrSelf()->PresentFlowCoordinator(this, nullptr, HMUI::ViewController::AnimationDirection::Horizontal, false, false);
    }

    void QosmeticsFlowCoordinator::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
    {
        HMUI::TitleViewController* titleView = Object::FindObjectOfType<HMUI::TitleViewController*>();
        if (firstActivation)
        {
            ProvideInitialViewControllers(qosmeticsViewController, profileSwitcherViewController, creditViewController, nullptr, nullptr);

            set_showBackButton(true);

            time_t rawtime = time(nullptr);
            tm localTime = *localtime(&rawtime);
            SetTitle("Qosmetics", HMUI::ViewController::AnimationType::In);

            if (DateUtils::isMonth(6))
            {
                titleGradientUpdater = get_gameObject()->AddComponent<BSML::TextGradientUpdater*>();
                titleGradientUpdater->set_gradient(BSML::Gradient::Parse(RainbowUtils::randomGradient()));
                titleGradientUpdater->text = titleView->text;
                titleGradientUpdater->scrollSpeed = 0.2;
                titleGradientUpdater->fixedStep = true;
                titleGradientUpdater->stepSize = 2;
            }
        }

        if (titleGradientUpdater && titleGradientUpdater->m_CachedPtr.m_value)
            titleGradientUpdater->set_enabled(true);

        creditViewController->get_gameObject()->SetActive(true);
        UIUtils::SetTitleColor(titleView, qosmetics_purple);
    }

    void QosmeticsFlowCoordinator::DidDeactivate(bool removedFromHierarchy, bool screenSystemDisabling)
    {
        if (!titleGradientUpdater || !titleGradientUpdater->m_CachedPtr.m_value)
            return;
        titleGradientUpdater->set_enabled(false);
        // reset title text to white
        HMUI::TitleViewController* titleView = Object::FindObjectOfType<HMUI::TitleViewController*>();
        auto text = titleView->text;
        auto textInfo = text->get_textInfo();
        auto characterCount = textInfo->characterCount;
        if (characterCount > 0)
        {
            int materialCount = textInfo->materialCount;
            for (int i = 0; i < materialCount; i++)
                for (auto& c : textInfo->meshInfo[i].colors32)
                    c = {255, 255, 255, 255};
            text->UpdateVertexData(TMPro::TMP_VertexDataUpdateFlags::Colors32);
        }
    }

    void QosmeticsFlowCoordinator::BackButtonWasPressed(HMUI::ViewController* topViewController)
    {
        creditViewController->get_gameObject()->SetActive(false);
        parentFlowCoordinator->DismissFlowCoordinator(this, HMUI::ViewController::AnimationDirection::Horizontal, nullptr, false);
        HMUI::TitleViewController* titleView = Object::FindObjectOfType<HMUI::TitleViewController*>();
        UIUtils::SetTitleColor(titleView, beatsaber_light_blue, true);
    }
}