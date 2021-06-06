#pragma once

#include "custom-types/shared/macros.hpp"

#include "HMUI/FlowCoordinator.hpp"
#include "HMUI/ViewController.hpp"

#include "UI/Saber/SaberPreviewViewController.hpp"
#include "UI/Saber/SaberSettingsViewController.hpp"
#include "UI/Saber/SaberSwitcherViewController.hpp"

#include "UI/Note/NotePreviewViewController.hpp"
#include "UI/Note/NoteSettingsViewController.hpp"
#include "UI/Note/NoteSwitcherViewController.hpp"

#include "UI/Wall/WallPreviewViewController.hpp"
#include "UI/Wall/WallSettingsViewController.hpp"
#include "UI/Wall/WallSwitcherViewController.hpp"

#include "UI/General/PatronViewController.hpp"
#include "UI/General/FloorLogoViewController.hpp"
#include "UI/General/QosmeticsViewController.hpp"
#include "UI/General/UserProfileViewController.hpp"

#include "UnityEngine/EventSystems/BaseInputModule.hpp"

DECLARE_CLASS_CODEGEN(Qosmetics::UI, QosmeticsFlowCoordinator, HMUI::FlowCoordinator,
    
    DECLARE_INSTANCE_FIELD_DEFAULT(Qosmetics::UI::QosmeticsViewController*, qosmeticsViewController, nullptr);

    DECLARE_INSTANCE_FIELD_DEFAULT(Qosmetics::UI::SaberSwitcherViewController*, saberSwitcherViewController, nullptr);
    DECLARE_INSTANCE_FIELD_DEFAULT(Qosmetics::UI::SaberSettingsViewController*, saberSettingsViewController, nullptr);
    DECLARE_INSTANCE_FIELD_DEFAULT(Qosmetics::UI::SaberPreviewViewController*, saberPreviewViewController, nullptr);

    DECLARE_INSTANCE_FIELD_DEFAULT(Qosmetics::UI::NoteSwitcherViewController*, noteSwitcherViewController, nullptr);
    DECLARE_INSTANCE_FIELD_DEFAULT(Qosmetics::UI::NoteSettingsViewController*, noteSettingsViewController, nullptr);
    DECLARE_INSTANCE_FIELD_DEFAULT(Qosmetics::UI::NotePreviewViewController*, notePreviewViewController, nullptr);

    DECLARE_INSTANCE_FIELD_DEFAULT(Qosmetics::UI::WallSwitcherViewController*, wallSwitcherViewController, nullptr);
    DECLARE_INSTANCE_FIELD_DEFAULT(Qosmetics::UI::WallSettingsViewController*, wallSettingsViewController, nullptr);
    DECLARE_INSTANCE_FIELD_DEFAULT(Qosmetics::UI::WallPreviewViewController*, wallPreviewViewController, nullptr);

    DECLARE_INSTANCE_FIELD_DEFAULT(Qosmetics::UI::PatronViewController*, patronViewController, nullptr);
    DECLARE_INSTANCE_FIELD_DEFAULT(Qosmetics::UI::FloorLogoViewController*, floorLogoViewController, nullptr);
    DECLARE_INSTANCE_FIELD_DEFAULT(Qosmetics::UI::UserProfileViewController*, userProfileViewController, nullptr);
    
    DECLARE_INSTANCE_FIELD_DEFAULT(Qosmetics::SaberManager*, saberManager, nullptr);
    DECLARE_INSTANCE_FIELD_DEFAULT(Qosmetics::NoteManager*, noteManager, nullptr);
    DECLARE_INSTANCE_FIELD_DEFAULT(Qosmetics::WallManager*, wallManager, nullptr);
    DECLARE_INSTANCE_FIELD_DEFAULT(Qosmetics::ColorManager*, colorManager, nullptr);

    DECLARE_INSTANCE_FIELD_DEFAULT(Zenject::DiContainer*, container, nullptr);

    DECLARE_OVERRIDE_METHOD(void, DidActivate, il2cpp_utils::FindMethodUnsafe("HMUI", "FlowCoordinator", "DidActivate", 3), bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);
    DECLARE_OVERRIDE_METHOD(void, BackButtonWasPressed, il2cpp_utils::FindMethodUnsafe("HMUI", "FlowCoordinator", "BackButtonWasPressed", 1), HMUI::ViewController* topViewController);
    
    DECLARE_METHOD(void, Init,  UnityEngine::EventSystems::BaseInputModule* inputModule,
                                SaberSwitcherViewController* saberSwitcherViewController,
                                SaberSettingsViewController* saberSettingsViewController,
                                SaberPreviewViewController* saberPreviewViewController,
                                NoteSwitcherViewController* noteSwitcherViewController,
                                NoteSettingsViewController* noteSettingsViewController,
                                NotePreviewViewController* notePreviewViewController,
                                WallSwitcherViewController* wallSwitcherViewController,
                                WallSettingsViewController* wallSettingsViewController,
                                WallPreviewViewController* wallPreviewViewController,
                                PatronViewController* patronViewController,
                                QosmeticsViewController* qosmeticsViewController,
                                FloorLogoViewController* floorLogoViewController,
                                UserProfileViewController* userProfileViewController);
    
    public:
        void SubMenuButtonWasPressed(ItemType type);

    REGISTER_FUNCTION(
        REGISTER_FIELD(qosmeticsViewController);

        REGISTER_FIELD(saberSwitcherViewController);
        REGISTER_FIELD(saberSettingsViewController);
        REGISTER_FIELD(saberPreviewViewController);

        REGISTER_FIELD(noteSwitcherViewController);
        REGISTER_FIELD(noteSettingsViewController);
        REGISTER_FIELD(notePreviewViewController);

        REGISTER_FIELD(wallSwitcherViewController);
        REGISTER_FIELD(wallSettingsViewController);
        REGISTER_FIELD(wallPreviewViewController);

        REGISTER_FIELD(patronViewController);
        REGISTER_FIELD(floorLogoViewController);
        REGISTER_FIELD(userProfileViewController);

        REGISTER_FIELD(saberManager);
        REGISTER_FIELD(noteManager);
        REGISTER_FIELD(wallManager);
        REGISTER_FIELD(colorManager);

        REGISTER_FIELD(container);

        REGISTER_METHOD(Init);
        REGISTER_METHOD(DidActivate);
        REGISTER_METHOD(BackButtonWasPressed);
    )
)