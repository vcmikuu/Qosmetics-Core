#include "UI/Saber/SaberDeletionElement.hpp"
#include "UI/Saber/SaberSelectionElement.hpp"
#include "questui/shared/BeatSaberUI.hpp"
DEFINE_TYPE(Qosmetics::UI, SaberDeletionElement);

using namespace QuestUI;
using namespace QuestUI::BeatSaberUI;

namespace Qosmetics::UI
{
    void SaberDeletionElement::Show(Qosmetics::UI::SaberSelectionElement* toDelete)
    {
        this->toDelete = toDelete;
        objectName->set_text(il2cpp_utils::createcsstr("<i>" + toDelete->get_descriptor().get_name() + "</i>"));
        modal->Show(true, true, nullptr);
    }

    void SaberDeletionElement::Confirm()
    {
        toDelete->Delete();
        modal->Hide(true, nullptr);
    }

    void SaberDeletionElement::Cancel()
    {
        modal->Hide(true, nullptr);
    }

    void SaberDeletionElement::Setup()
    {
        modal = GetComponent<HMUI::ModalView*>();

        //UnityEngine::GameObject* container = CreateScrollableModalContainer(modal);
        auto wrapper = CreateHorizontalLayoutGroup(get_transform());
        auto container = CreateVerticalLayoutGroup(wrapper->get_transform());
        container->set_childAlignment(UnityEngine::TextAnchor::MiddleCenter);
        CreateText(container->get_transform(), "Are you sure you want to delete")->set_alignment(TMPro::TextAlignmentOptions::Center);

        objectName = CreateText(container->get_transform(), "PLACEHOLDER");
        objectName->set_alignment(TMPro::TextAlignmentOptions::Center);

        auto horizon = CreateHorizontalLayoutGroup(container->get_transform());

        CreateUIButton(horizon->get_transform(), "<color=#ff0000>confirm</color>", [this]() -> void {
            this->Confirm();
        });

        CreateUIButton(horizon->get_transform(), "<color=#00ff00>cancel</color>", [this]() -> void {
            this->Cancel();
        });
    }
}