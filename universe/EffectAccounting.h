#ifndef _Effect_Accounting_h_
#define _Effect_Accounting_h_


#include "EnumsFwd.h"

#include <boost/serialization/access.hpp>

#include <memory>
#include <map>
#include <string>
#include <vector>

#include "../util/Export.h"

class UniverseObject;
struct ScriptingContext;

namespace Effect {
    struct AccountingInfo;
    class EffectsGroup;

    typedef std::vector<std::shared_ptr<UniverseObject>> TargetSet;
    /** Effect accounting information for all meters of all objects that are
      * acted on by effects. */
    typedef std::map<int, std::map<MeterType, std::vector<AccountingInfo>>> AccountingMap;


    /** Description of cause of an effect: the general cause type, and the
      * specific cause.  eg. Building and a particular BuildingType. */
    struct FO_COMMON_API EffectCause {
        EffectCause();
        EffectCause(EffectsCauseType cause_type_, const std::string& specific_cause_,
                    const std::string& custom_label_ = "");
        EffectsCauseType    cause_type;     ///< general type of effect cause, eg. tech, building, special...
        std::string         specific_cause; ///< name of specific cause, eg. "Wonder Farm", "Antenna Mk. VI"
        std::string         custom_label;   ///< script-specified accounting label for this effect cause
    };

    /** Combination of targets and cause for an effects group. */
    struct TargetsAndCause {
        TargetsAndCause();
        TargetsAndCause(const TargetSet& target_set_, const EffectCause& effect_cause_);
        TargetSet target_set;
        EffectCause effect_cause;
    };

    /** Combination of an EffectsGroup and the id of a source object. */
    struct SourcedEffectsGroup {
        SourcedEffectsGroup();
        SourcedEffectsGroup(int source_object_id_, const std::shared_ptr<EffectsGroup>& effects_group_);
        bool operator<(const SourcedEffectsGroup& right) const;
        int source_object_id;
        std::shared_ptr<EffectsGroup> effects_group;
    };

    /** Map from (effects group and source object) to target set of for
      * that effects group with that source object.  A multimap is used
      * so that a single source object can have multiple instances of the
      * same effectsgroup.  This is useful when a Ship has multiple copies
      * of the same effects group due to having multiple copies of the same
      * ship part in its design. */
    typedef std::vector<std::pair<SourcedEffectsGroup, TargetsAndCause>> TargetsCauses;

    /** The base class for all Effects.  When an Effect is executed, the source
    * object (the object to which the Effect or its containing EffectGroup is
    * attached) and the target object are both required.  Note that this means
    * that ValueRefs contained within Effects can refer to values in either the
    * source or target objects. */
    class FO_COMMON_API EffectBase {
    public:
        virtual ~EffectBase();

        virtual void Execute(const ScriptingContext& context) const = 0;

        virtual void Execute(const ScriptingContext& context, const TargetSet& targets) const;

        void Execute(const TargetsCauses& targets_causes,
                    AccountingMap* accounting_map,
                    bool only_meter_effects = false,
                    bool only_appearance_effects = false,
                    bool include_empire_meter_effects = false,
                    bool only_generate_sitrep_effects = false) const;

        virtual void Execute(const ScriptingContext& context,
                            const TargetSet& targets,
                            AccountingMap* accounting_map,
                            const EffectCause& effect_cause,
                            bool only_meter_effects = false,
                            bool only_appearance_effects = false,
                            bool include_empire_meter_effects = false,
                            bool only_generate_sitrep_effects = false) const;

        virtual std::string     Dump(unsigned short ntabs = 0) const = 0;
        virtual bool            IsMeterEffect() const { return false; }
        virtual bool            IsEmpireMeterEffect() const { return false; }
        virtual bool            IsAppearanceEffect() const { return false; }
        virtual bool            IsSitrepEffect() const { return false; }
        virtual bool            IsConditionalEffect() const { return false; }
        virtual void            SetTopLevelContent(const std::string& content_name) = 0;
        virtual unsigned int    GetCheckSum() const;

    private:
        friend class boost::serialization::access;
        template <class Archive>
        void serialize(Archive& ar, const unsigned int version);
    };

    /** Accounting information about what the causes are and changes produced
      * by effects groups acting on meters of objects. */
    struct FO_COMMON_API AccountingInfo : public EffectCause {
        AccountingInfo();
        bool operator==(const AccountingInfo& rhs) const;

        int     source_id;          ///< source object of effect
        float   meter_change;       ///< net change on meter due to this effect, as best known by client's empire
        float   running_meter_total;///< meter total as of this effect.
    };

    /** Discrepancy between meter's value at start of turn, and the value that
      * this client calculate that the meter should have with the knowledge
      * available -> the unknown factor affecting the meter.  This is used
      * when generating effect accounting, in the case where the expected
      * and actual meter values don't match. */
    typedef std::map<int, std::map<MeterType, double>> DiscrepancyMap;
}

#endif
