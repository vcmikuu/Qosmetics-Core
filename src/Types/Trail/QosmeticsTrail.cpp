#include "Types/Trail/QosmeticsTrail.hpp"
#include "GlobalNamespace/IBladeMovementData.hpp"
#include "GlobalNamespace/TimeHelper.hpp"

#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/MeshRenderer.hpp"
#include "UnityEngine/Vector3.hpp"

#include "Config.hpp"
#include "Utils/TrailUtils.hpp"
#include "QosmeticsLogger.hpp"

extern config_t config;

DEFINE_CLASS(Qosmetics::QosmeticsTrail);

using namespace UnityEngine;

static Il2CppString* topTransformName = nullptr;
static Il2CppString* bottomTransformName = nullptr;
static Il2CppString* customBottomTransformName = nullptr;

#define INFO(value...) QosmeticsLogger::GetContextLogger("Trail").info(value)
#define ERROR(value...) QosmeticsLogger::GetContextLogger("Trail").error(value)

float Qosmetics::QosmeticsTrail::trailIntensity = 1.0f;

namespace Qosmetics
{
    void QosmeticsTrail::Awake()
    {
        if (!trailMaterial)
        {
            MeshRenderer* renderer = get_gameObject()->GetComponent<MeshRenderer*>();
            if (renderer) trailMaterial = renderer->get_sharedMaterial();
        }

        if (!this->customMovementData) this->customMovementData = GlobalNamespace::SaberMovementData::New_ctor();
        this->movementData = reinterpret_cast<GlobalNamespace::IBladeMovementData*>(this->customMovementData);

        if (!this->trailRenderer) this->trailRenderer = this->trailRendererPrefab = NewTrailRenderer();

        this->trailDuration = (float)this->length / (float)this->samplingFrequency;
        this->whiteSectionMaxDuration = (float)this->whitestep / (float)this->samplingFrequency;

        if (!topTransformName) topTransformName = il2cpp_utils::createcsstr("TrailEnd", il2cpp_utils::StringType::Manual);
        if (!bottomTransformName) bottomTransformName = il2cpp_utils::createcsstr("TrailStart", il2cpp_utils::StringType::Manual);
        if (!customBottomTransformName) customBottomTransformName = il2cpp_utils::createcsstr("CustomTrailStart", il2cpp_utils::StringType::Manual);

        this->topTransform = get_transform()->Find(topTransformName);

        if (!this->topTransform)
        {
            Transform* newTop = GameObject::New_ctor()->get_transform();
            newTop->SetParent(get_transform());
            newTop->set_localPosition({0.0f, 0.0f, 1.0f});
            newTop->get_gameObject()->set_name(il2cpp_utils::createcsstr("TrailEnd"));
        }

        this->bottomTransform = get_transform()->Find(bottomTransformName);

        if (!this->bottomTransform)
        {
            Transform* newBottom = GameObject::New_ctor()->get_transform();
            newBottom->SetParent(get_transform());
            newBottom->set_localPosition({0.0f, 0.0f, 0.0f});
            newBottom->get_gameObject()->set_name(bottomTransformName);
        }

        this->customBottomTransform = get_transform()->Find(customBottomTransformName);

        
        if (!this->customBottomTransform)
        {
            this->customBottomTransform = UnityEngine::Object::Instantiate(this->bottomTransform, get_transform());
		    this->customBottomTransform->set_name(customBottomTransformName);
        }
        

        if (config.saberConfig.overrideTrailWidth && this->customBottomTransform)
		{
			UnityEngine::Vector3 newPos = UnityEngine::Vector3::Lerp(this->topTransform->get_localPosition(), this->bottomTransform->get_localPosition(), config.saberConfig.trailWidth);
			this->customBottomTransform->set_localPosition(newPos);
		}
    }

    void QosmeticsTrail::InitTrail(int length, int colorType, int whiteStep, UnityEngine::Material* material, UnityEngine::Color trailColor, UnityEngine::Color multiplierColor, bool ignoreOverrides = false)
    {
        INFO("Initing trail");
        customInited = false;
        int overriddenLength = (int)config.saberConfig.trailLength;
        this->length = (config.saberConfig.overrideTrailLength && !ignoreOverrides) ? overriddenLength : length;
        
        this->granularity = (int)(60.0f * ((this->length > 10.0f) ? this->length / 10.0f : 1.0f));
        this->colorType = (config.saberConfig.whiteTrail && !ignoreOverrides) ? 2 : colorType;
        
        int overriddenWhitestep = (int)config.saberConfig.whiteStep * this->length;
		this->whitestep = (config.saberConfig.overrideWhiteStep && !ignoreOverrides) ? overriddenWhitestep : whiteStep;
		
        this->trailMaterial = material;
		this->trailColor = (config.saberConfig.whiteTrail && !ignoreOverrides) ? UnityEngine::Color::get_white() : trailColor;
		this->multiplierColor = multiplierColor;

        if (!this->trailMaterial)
        {
            MeshRenderer* renderer = get_gameObject()->GetComponent<MeshRenderer*>();
            if (renderer) this->trailMaterial = renderer->get_sharedMaterial();
        }

        this->trailRenderer = this->trailRendererPrefab = NewTrailRenderer();

        if (!topTransformName) topTransformName = il2cpp_utils::createcsstr("TrailEnd", il2cpp_utils::StringType::Manual);
        if (!bottomTransformName) bottomTransformName = il2cpp_utils::createcsstr("TrailStart", il2cpp_utils::StringType::Manual);
        if (!customBottomTransformName) customBottomTransformName = il2cpp_utils::createcsstr("CustomTrailStart", il2cpp_utils::StringType::Manual);

        this->topTransform = get_transform()->Find(topTransformName);
        this->bottomTransform = get_transform()->Find(bottomTransformName);
        this->customBottomTransform = get_transform()->Find(customBottomTransformName);
        if (!customBottomTransform)
        {
            this->customBottomTransform = UnityEngine::Object::Instantiate(this->bottomTransform, get_transform());
			this->customBottomTransform->set_name(customBottomTransformName);
        }

        UpdateTrail();
        customInited = true;
    }

    void QosmeticsTrail::InitFromDefault(UnityEngine::Transform* objToCopy)
    {
        if (!objToCopy) return;
        GlobalNamespace::SaberTrail* orig = objToCopy->GetComponent<GlobalNamespace::SaberTrail*>();
        if (!orig) return;

        int length = (int)((float)orig->samplingFrequency * orig->trailDuration * 0.5f);
		int colorType = 2;
		int whiteStep = (int)((float)orig->samplingFrequency * orig->whiteSectionMaxDuration * 0.5f);
		UnityEngine::Material* mat = orig->trailRendererPrefab->meshRenderer->get_material();
		this->trailRenderer = TrailUtils::NewTrailRenderer(mat);
		UnityEngine::Color trailColor = orig->color;
		UnityEngine::Color multiplier = UnityEngine::Color::get_white();

        InitTrail(  length,
                    colorType,
                    whiteStep,
                    mat,
                    trailColor,
                    multiplier
        );
    }

    void QosmeticsTrail::Update()
    {
        if (!customInited) return;
        if (!this->topTransform || !this->bottomTransform) return;
        if (!this->customMovementData) return;

        UnityEngine::Vector3 topPos = this->topTransform->get_position();
		UnityEngine::Vector3 bottomPos = (config.saberConfig.overrideTrailWidth && this->customBottomTransform && false) ? this->customBottomTransform->get_position() : this->bottomTransform->get_position();

        this->customMovementData->AddNewData(topPos, bottomPos, GlobalNamespace::TimeHelper::get_time());
    }

    void QosmeticsTrail::UpdateTrail()
    {
        if (!this->customMovementData) this->customMovementData = GlobalNamespace::SaberMovementData::New_ctor();
        this->movementData = reinterpret_cast<GlobalNamespace::IBladeMovementData*>(this->customMovementData);

        this->trailDuration = (float)this->length / (float)this->samplingFrequency;
        this->whiteSectionMaxDuration = (float)this->whitestep / (float)this->samplingFrequency;
        
        // change trail width if enabled
        if (config.saberConfig.overrideTrailWidth && this->customBottomTransform)
		{
			Vector3 newPos = Vector3::Lerp(this->topTransform->get_localPosition(), this->bottomTransform->get_localPosition(), config.saberConfig.trailWidth);
			this->customBottomTransform->set_localPosition(newPos);
		}
        // if there is any issue with these objects, they will be remade
        if (!this->trailRenderer || !this->trailRenderer->meshRenderer->get_sharedMaterial())
		{
			MeshRenderer* renderer = this->get_gameObject()->GetComponent<MeshRenderer*>();
			if (!this->trailMaterial && renderer)
			{
				this->trailMaterial = renderer->get_sharedMaterial();
				this->trailRenderer = this->trailRendererPrefab = NewTrailRenderer();
			}
		}

        UpdateColors();
    }

    void QosmeticsTrail::UpdateColors()
    {
       switch (this->colorType)
		{
			case 0: // LeftSaber
				this->color = colorManager->ColorForTrailType(0) * this->multiplierColor;
				break;
			case 1:	// RightSaber
				this->color = colorManager->ColorForTrailType(1) * this->multiplierColor;
				break;
			default:	// Custom Color
				this->color = this->trailColor * this->multiplierColor;
				break;
		}
        this->color.a *= trailIntensity;
    }

    void QosmeticsTrail::SetTrailConfig(TrailConfig* config)
    {
        if (!config) return;
        trailConfig = config;
        Reset();
    }

    void QosmeticsTrail::SetColorManager(ColorManager* colorManager)
    {
        this->colorManager = colorManager;
        std::function<void()> callback = std::bind( &QosmeticsTrail::UpdateColors, this );
        this->colorManager->RegisterCallback(callback, callbackType::trail);
        UpdateColors();
    }

    void QosmeticsTrail::Reset()
    {
        if (this->trailConfig)
        {
            InitTrail(  trailConfig->get_length(),
                        (int)trailConfig->get_colorType(),
                        trailConfig->get_whiteStep(),
                        trailMaterial,
                        trailConfig->get_trailColor(),
                        trailConfig->get_multiplierColor()
                );
        }
        else
        {
            InitTrail(  length,
                        colorType,
                        whitestep,
                        trailMaterial,
                        trailColor,
                        multiplierColor
                );
        }
    }

    GlobalNamespace::SaberTrailRenderer* QosmeticsTrail::NewTrailRenderer()
    {
        GlobalNamespace::SaberTrailRenderer* newRenderer = TrailUtils::NewTrailRenderer(trailMaterial);
        float trailWidth = GetTrailWidth(movementData->get_lastAddedData());
        newRenderer->Init(trailWidth, trailDuration, granularity, whiteSectionMaxDuration);
        return newRenderer;
    }
}