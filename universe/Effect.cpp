#include "Effect.h"

#include "UniverseObject.h"
#include "ObjectMap.h"
#include "Enums.h"
#include "Condition.h"

#include "../util/CheckSums.h"

Effect::EffectCause::EffectCause() :
    cause_type(INVALID_EFFECTS_GROUP_CAUSE_TYPE),
    specific_cause(),
    custom_label()
{}

Effect::EffectCause::EffectCause(EffectsCauseType cause_type_, const std::string& specific_cause_,
                                 const std::string& custom_label_) :
    cause_type(cause_type_),
    specific_cause(specific_cause_),
    custom_label(custom_label_)
{
    //DebugLogger() << "EffectCause(" << cause_type << ", " << specific_cause << ", " << custom_label << ")";
}

Effect::AccountingInfo::AccountingInfo() :
    EffectCause(),
    source_id(INVALID_OBJECT_ID),
    meter_change(0.0),
    running_meter_total(0.0)
{}

bool Effect::AccountingInfo::operator==(const AccountingInfo& rhs) const {
    return
        cause_type == rhs.cause_type &&
        specific_cause == rhs.specific_cause &&
        custom_label == rhs.custom_label &&
        source_id == rhs.source_id &&
        meter_change == rhs.meter_change &&
        running_meter_total == rhs.running_meter_total;
}

Effect::TargetsAndCause::TargetsAndCause() :
    target_set(),
    effect_cause()
{}

Effect::TargetsAndCause::TargetsAndCause(const TargetSet& target_set_, const EffectCause& effect_cause_) :
    target_set(target_set_),
    effect_cause(effect_cause_)
{}

Effect::SourcedEffectsGroup::SourcedEffectsGroup() :
    source_object_id(INVALID_OBJECT_ID)
{}

Effect::SourcedEffectsGroup::SourcedEffectsGroup(int source_object_id_, const std::shared_ptr<EffectsGroup>& effects_group_) :
    source_object_id(source_object_id_),
    effects_group(effects_group_)
{}

bool Effect::SourcedEffectsGroup::operator<(const SourcedEffectsGroup& right) const {
    return (this->source_object_id < right.source_object_id ||
            ((this->source_object_id == right.source_object_id) && this->effects_group < right.effects_group));
}

///////////////////////////////////////////////////////////
// EffectsGroup                                          //
///////////////////////////////////////////////////////////
Effect::EffectsGroup::EffectsGroup(std::unique_ptr<Condition::Condition>&& scope,
                           std::unique_ptr<Condition::Condition>&& activation,
                           std::vector<std::unique_ptr<Effect>>&& effects,
                           const std::string& accounting_label,
                           const std::string& stacking_group, int priority,
                           const std::string& description,
                           const std::string& content_name):
    m_scope(std::move(scope)),
    m_activation(std::move(activation)),
    m_stacking_group(stacking_group),
    m_effects(std::move(effects)),
    m_accounting_label(accounting_label),
    m_priority(priority),
    m_description(description),
    m_content_name(content_name)
{}

Effect::EffectsGroup::~EffectsGroup()
{}

void Effect::EffectsGroup::Execute(const TargetsCauses& targets_causes, AccountingMap* accounting_map,
                           bool only_meter_effects, bool only_appearance_effects,
                           bool include_empire_meter_effects,
                           bool only_generate_sitrep_effects) const
{
    // execute each effect of the group one by one, unless filtered by flags
    for (auto& effect : m_effects) {
        effect->Execute(targets_causes, accounting_map,
                        only_meter_effects, only_appearance_effects,
                        include_empire_meter_effects,
                        only_generate_sitrep_effects);
    }
}

const std::vector<Effect::Effect*>  Effect::EffectsGroup::EffectsList() const {
    std::vector<Effect*> retval(m_effects.size());
    std::transform(m_effects.begin(), m_effects.end(), retval.begin(),
                   [](const std::unique_ptr<Effect>& xx) {return xx.get();});
    return retval;
}

const std::string& Effect::EffectsGroup::GetDescription() const
{ return m_description; }

std::string Effect::EffectsGroup::Dump(unsigned short ntabs) const {
    std::string retval = DumpIndent(ntabs) + "EffectsGroup";
    if (!m_content_name.empty())
        retval += " // from " + m_content_name;
    retval += "\n";
    retval += DumpIndent(ntabs+1) + "scope =\n";
    retval += m_scope->Dump(ntabs+2);
    if (m_activation) {
        retval += DumpIndent(ntabs+1) + "activation =\n";
        retval += m_activation->Dump(ntabs+2);
    }
    if (!m_stacking_group.empty())
        retval += DumpIndent(ntabs+1) + "stackinggroup = \"" + m_stacking_group + "\"\n";
    if (m_effects.size() == 1) {
        retval += DumpIndent(ntabs+1) + "effects =\n";
        retval += m_effects[0]->Dump(ntabs+2);
    } else {
        retval += DumpIndent(ntabs+1) + "effects = [\n";
        for (auto& effect : m_effects) {
            retval += effect->Dump(ntabs+2);
        }
        retval += DumpIndent(ntabs+1) + "]\n";
    }
    return retval;
}

bool Effect::EffectsGroup::HasMeterEffects() const {
    for (auto& effect : m_effects) {
        if (effect->IsMeterEffect())
            return true;
    }
    return false;
}

bool Effect::EffectsGroup::HasAppearanceEffects() const {
    for (auto& effect : m_effects) {
        if (effect->IsAppearanceEffect())
            return true;
    }
    return false;
}

bool Effect::EffectsGroup::HasSitrepEffects() const {
    for (auto& effect : m_effects) {
        if (effect->IsSitrepEffect())
            return true;
    }
    return false;
}

void Effect::EffectsGroup::SetTopLevelContent(const std::string& content_name) {
    m_content_name = content_name;
    if (m_scope)
        m_scope->SetTopLevelContent(content_name);
    if (m_activation)
        m_activation->SetTopLevelContent(content_name);
    for (auto& effect : m_effects) {
        effect->SetTopLevelContent(content_name);
    }
}

unsigned int Effect::EffectsGroup::GetCheckSum() const {
    unsigned int retval{0};

    CheckSums::CheckSumCombine(retval, "EffectsGroup");
    CheckSums::CheckSumCombine(retval, m_scope);
    CheckSums::CheckSumCombine(retval, m_activation);
    CheckSums::CheckSumCombine(retval, m_stacking_group);
    CheckSums::CheckSumCombine(retval, m_effects);
    CheckSums::CheckSumCombine(retval, m_accounting_label);
    CheckSums::CheckSumCombine(retval, m_priority);
    CheckSums::CheckSumCombine(retval, m_description);

    TraceLogger() << "GetCheckSum(EffectsGroup): retval: " << retval;
    return retval;
}

///////////////////////////////////////////////////////////
// Dump function                                         //
///////////////////////////////////////////////////////////
std::string Effect::Dump(const std::vector<std::shared_ptr<EffectsGroup>>& effects_groups) {
    std::stringstream retval;

    for (auto& effects_group : effects_groups) {
        retval << "\n" << effects_group->Dump();
    }

    return retval.str();
}
