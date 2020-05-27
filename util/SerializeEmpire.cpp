#include "Serialize.h"

#include "AppInterface.h"
#include "SitRepEntry.h"
#include "../Empire/Empire.h"
#include "../Empire/EmpireManager.h"
#include "../Empire/Supply.h"
#include "../Empire/Diplomacy.h"
#include "../universe/Universe.h"

#include "GameRules.h"

#include "Serialize.ipp"
#include <boost/serialization/version.hpp>
#include <boost/uuid/random_generator.hpp>


template <typename Archive>
void load_construct_data(Archive& ar, ResourcePool* respool, unsigned int const version)
{ ::new(respool)ResourcePool(INVALID_RESOURCE_TYPE); }

template <typename Archive>
void serialize(Archive& ar, ResourcePool& respool, unsigned int const version)
{
    using namespace boost::serialization;

    ar  & make_nvp("m_type", respool.m_type)
        & make_nvp("m_object_ids", respool.m_object_ids)
        & make_nvp("m_stockpile", respool.m_stockpile);
    if (version < 1) {
        int dummy = -1;
        ar  & make_nvp("m_stockpile_object_id", dummy);
    }
    ar  & make_nvp("m_connected_system_groups", respool.m_connected_system_groups);
}

BOOST_CLASS_VERSION(ResourcePool, 1)

template void serialize<freeorion_bin_oarchive>(freeorion_bin_oarchive&, ResourcePool&, const unsigned int);
template void serialize<freeorion_bin_iarchive>(freeorion_bin_iarchive&, ResourcePool&, const unsigned int);
template void serialize<freeorion_xml_oarchive>(freeorion_xml_oarchive&, ResourcePool&, const unsigned int);
template void serialize<freeorion_xml_iarchive>(freeorion_xml_iarchive&, ResourcePool&, const unsigned int);


template <typename Archive>
void serialize(Archive& ar, PopulationPool& poppool, unsigned int const version)
{
    ar  & boost::serialization::make_nvp("m_pop_center_ids", poppool.m_pop_center_ids);
}

template void serialize<freeorion_bin_oarchive>(freeorion_bin_oarchive&, PopulationPool&, const unsigned int);
template void serialize<freeorion_bin_iarchive>(freeorion_bin_iarchive&, PopulationPool&, const unsigned int);
template void serialize<freeorion_xml_oarchive>(freeorion_xml_oarchive&, PopulationPool&, const unsigned int);
template void serialize<freeorion_xml_iarchive>(freeorion_xml_iarchive&, PopulationPool&, const unsigned int);


template <typename Archive>
void serialize(Archive& ar, ResearchQueue::Element& obj, unsigned int const version)
{
    using namespace boost::serialization;

    ar  & make_nvp("name", obj.name)
        & make_nvp("empire_id", obj.empire_id)
        & make_nvp("allocated_rp", obj.allocated_rp)
        & make_nvp("turns_left", obj.turns_left)
        & make_nvp("paused", obj.paused);
}

template void serialize<freeorion_bin_oarchive>(freeorion_bin_oarchive&, ResearchQueue::Element&, unsigned int const);
template void serialize<freeorion_bin_iarchive>(freeorion_bin_iarchive&, ResearchQueue::Element&, unsigned int const);
template void serialize<freeorion_xml_oarchive>(freeorion_xml_oarchive&, ResearchQueue::Element&, unsigned int const);
template void serialize<freeorion_xml_iarchive>(freeorion_xml_iarchive&, ResearchQueue::Element&, unsigned int const);


template <typename Archive>
void serialize(Archive& ar, ResearchQueue& obj, unsigned int const version)
{
    using namespace boost::serialization;

    ar  & make_nvp("m_queue", obj.m_queue)
        & make_nvp("m_projects_in_progress", obj.m_projects_in_progress)
        & make_nvp("m_total_RPs_spent", obj.m_total_RPs_spent)
        & make_nvp("m_empire_id", obj.m_empire_id);
}

template void serialize<freeorion_bin_oarchive>(freeorion_bin_oarchive&, ResearchQueue&, unsigned int const);
template void serialize<freeorion_bin_iarchive>(freeorion_bin_iarchive&, ResearchQueue&, unsigned int const);
template void serialize<freeorion_xml_oarchive>(freeorion_xml_oarchive&, ResearchQueue&, unsigned int const);
template void serialize<freeorion_xml_iarchive>(freeorion_xml_iarchive&, ResearchQueue&, unsigned int const);


template <typename Archive>
void ProductionQueue::ProductionItem::serialize(Archive& ar, const unsigned int version)
{
    ar  & BOOST_SERIALIZATION_NVP(build_type)
        & BOOST_SERIALIZATION_NVP(name)
        & BOOST_SERIALIZATION_NVP(design_id);
}

template void ProductionQueue::ProductionItem::serialize<freeorion_bin_oarchive>(freeorion_bin_oarchive&, const unsigned int);
template void ProductionQueue::ProductionItem::serialize<freeorion_bin_iarchive>(freeorion_bin_iarchive&, const unsigned int);
template void ProductionQueue::ProductionItem::serialize<freeorion_xml_oarchive>(freeorion_xml_oarchive&, const unsigned int);
template void ProductionQueue::ProductionItem::serialize<freeorion_xml_iarchive>(freeorion_xml_iarchive&, const unsigned int);

template <typename Archive>
void ProductionQueue::Element::serialize(Archive& ar, const unsigned int version)
{
    ar  & BOOST_SERIALIZATION_NVP(item)
        & BOOST_SERIALIZATION_NVP(empire_id)
        & BOOST_SERIALIZATION_NVP(ordered)
        & BOOST_SERIALIZATION_NVP(remaining)
        & BOOST_SERIALIZATION_NVP(blocksize)
        & BOOST_SERIALIZATION_NVP(location)
        & BOOST_SERIALIZATION_NVP(allocated_pp)
        & BOOST_SERIALIZATION_NVP(progress)
        & BOOST_SERIALIZATION_NVP(progress_memory)
        & BOOST_SERIALIZATION_NVP(blocksize_memory)
        & BOOST_SERIALIZATION_NVP(turns_left_to_next_item)
        & BOOST_SERIALIZATION_NVP(turns_left_to_completion)
        & BOOST_SERIALIZATION_NVP(rally_point_id)
        & BOOST_SERIALIZATION_NVP(paused)
        & BOOST_SERIALIZATION_NVP(allowed_imperial_stockpile_use);

    if (Archive::is_saving::value) {
        // Serialization of uuid as a primitive doesn't work as expected from
        // the documentation.  This workaround instead serializes a string
        // representation.
        auto string_uuid = boost::uuids::to_string(uuid);
        ar & BOOST_SERIALIZATION_NVP(string_uuid);

     } else if (Archive::is_loading::value && version < 2) {
        // assign a random ID to this element so that future-issued orders can refer to it
        uuid = boost::uuids::random_generator()();

    } else {
        // convert string back into UUID
        std::string string_uuid;
        ar & BOOST_SERIALIZATION_NVP(string_uuid);

        try {
            uuid = boost::lexical_cast<boost::uuids::uuid>(string_uuid);
        } catch (const boost::bad_lexical_cast&) {
            uuid = boost::uuids::random_generator()();
        }
    }
}

BOOST_CLASS_VERSION(ProductionQueue::Element, 2)

template void ProductionQueue::Element::serialize<freeorion_bin_oarchive>(freeorion_bin_oarchive&, const unsigned int);
template void ProductionQueue::Element::serialize<freeorion_bin_iarchive>(freeorion_bin_iarchive&, const unsigned int);
template void ProductionQueue::Element::serialize<freeorion_xml_oarchive>(freeorion_xml_oarchive&, const unsigned int);
template void ProductionQueue::Element::serialize<freeorion_xml_iarchive>(freeorion_xml_iarchive&, const unsigned int);

template <typename Archive>
void ProductionQueue::serialize(Archive& ar, const unsigned int version)
{
    ar  & BOOST_SERIALIZATION_NVP(m_queue)
        & BOOST_SERIALIZATION_NVP(m_projects_in_progress)
        & BOOST_SERIALIZATION_NVP(m_object_group_allocated_pp)
        & BOOST_SERIALIZATION_NVP(m_object_group_allocated_stockpile_pp)
        & BOOST_SERIALIZATION_NVP(m_expected_new_stockpile_amount)
        & BOOST_SERIALIZATION_NVP(m_empire_id);
}

template void ProductionQueue::serialize<freeorion_bin_oarchive>(freeorion_bin_oarchive&, const unsigned int);
template void ProductionQueue::serialize<freeorion_bin_iarchive>(freeorion_bin_iarchive&, const unsigned int);
template void ProductionQueue::serialize<freeorion_xml_oarchive>(freeorion_xml_oarchive&, const unsigned int);
template void ProductionQueue::serialize<freeorion_xml_iarchive>(freeorion_xml_iarchive&, const unsigned int);

template <typename Archive>
void serialize(Archive& ar, InfluenceQueue::Element& obj, unsigned int const version)
{
    using namespace boost::serialization;

    ar  & make_nvp("name", obj.name)
        & make_nvp("empire_id", obj.empire_id)
        & make_nvp("allocated_ip", obj.allocated_ip)
        & make_nvp("paused", obj.paused);
}

template void serialize<freeorion_bin_oarchive>(freeorion_bin_oarchive&, InfluenceQueue::Element&, unsigned int const);
template void serialize<freeorion_bin_iarchive>(freeorion_bin_iarchive&, InfluenceQueue::Element&, unsigned int const);
template void serialize<freeorion_xml_oarchive>(freeorion_xml_oarchive&, InfluenceQueue::Element&, unsigned int const);
template void serialize<freeorion_xml_iarchive>(freeorion_xml_iarchive&, InfluenceQueue::Element&, unsigned int const);


template <class Archive>
void serialize(Archive& ar, InfluenceQueue& obj, unsigned int const version)
{
    using namespace boost::serialization;

    ar  & make_nvp("m_queue", obj.m_queue)
        & make_nvp("m_projects_in_progress", obj.m_projects_in_progress)
        & make_nvp("m_total_IPs_spent", obj.m_total_IPs_spent)
        & make_nvp("m_empire_id", obj.m_empire_id);
}

template void serialize<freeorion_bin_oarchive>(freeorion_bin_oarchive&, InfluenceQueue&, unsigned int const);
template void serialize<freeorion_bin_iarchive>(freeorion_bin_iarchive&, InfluenceQueue&, unsigned int const);
template void serialize<freeorion_xml_oarchive>(freeorion_xml_oarchive&, InfluenceQueue&, unsigned int const);
template void serialize<freeorion_xml_iarchive>(freeorion_xml_iarchive&, InfluenceQueue&, unsigned int const);


template <class Archive>
void Empire::PolicyAdoptionInfo::serialize(Archive& ar, const unsigned int version)
{
    ar  & BOOST_SERIALIZATION_NVP(adoption_turn)
        & BOOST_SERIALIZATION_NVP(category)
        & BOOST_SERIALIZATION_NVP(slot_in_category);
}

template void Empire::PolicyAdoptionInfo::serialize<freeorion_bin_oarchive>(freeorion_bin_oarchive&, const unsigned int);
template void Empire::PolicyAdoptionInfo::serialize<freeorion_bin_iarchive>(freeorion_bin_iarchive&, const unsigned int);
template void Empire::PolicyAdoptionInfo::serialize<freeorion_xml_oarchive>(freeorion_xml_oarchive&, const unsigned int);
template void Empire::PolicyAdoptionInfo::serialize<freeorion_xml_iarchive>(freeorion_xml_iarchive&, const unsigned int);

template <class Archive>
void Empire::serialize(Archive& ar, const unsigned int version)
{
    ar  & BOOST_SERIALIZATION_NVP(m_id)
        & BOOST_SERIALIZATION_NVP(m_name)
        & BOOST_SERIALIZATION_NVP(m_player_name)
        & BOOST_SERIALIZATION_NVP(m_color)
        & BOOST_SERIALIZATION_NVP(m_capital_id)
        & BOOST_SERIALIZATION_NVP(m_source_id)
        & BOOST_SERIALIZATION_NVP(m_eliminated)
        & BOOST_SERIALIZATION_NVP(m_victories);

    bool visible = GetUniverse().AllObjectsVisible() ||
        GetUniverse().EncodingEmpire() == ALL_EMPIRES ||
        m_id == GetUniverse().EncodingEmpire();
    bool allied_visible = visible || Empires().GetDiplomaticStatus(m_id, GetUniverse().EncodingEmpire()) == DIPLO_ALLIED;

    if (Archive::is_loading::value && version < 1) {
        // adapt set to map
        std::set<std::string> temp_stringset;
        ar  & boost::serialization::make_nvp("m_techs", temp_stringset);
        m_techs.clear();
        for (auto& entry : temp_stringset)
            m_techs[entry] = BEFORE_FIRST_TURN;
    } else {
        ar  & BOOST_SERIALIZATION_NVP(m_techs);

        if (Archive::is_loading::value && version < 4) {
            m_adopted_policies.clear();
            m_initial_adopted_policies.clear();
            m_available_policies.clear();
            m_policy_adoption_total_duration.clear();
        } else {
            ar  & BOOST_SERIALIZATION_NVP(m_adopted_policies)
                & BOOST_SERIALIZATION_NVP(m_initial_adopted_policies)
                & BOOST_SERIALIZATION_NVP(m_available_policies)
                & BOOST_SERIALIZATION_NVP(m_policy_adoption_total_duration);
        }
    }

    ar  & BOOST_SERIALIZATION_NVP(m_meters);
    if (Archive::is_saving::value && !allied_visible) {
        // don't send what other empires building and researching
        // and which building and ship parts are available to them
        ResearchQueue empty_research_queue(m_id);
        std::map<std::string, float> empty_research_progress;
        ProductionQueue empty_production_queue(m_id);
        std::set<std::string> empty_string_set;
        InfluenceQueue empty_influence_queue(m_id);
        ar  & boost::serialization::make_nvp("m_research_queue", empty_research_queue)
            & boost::serialization::make_nvp("m_research_progress", empty_research_progress)
            & boost::serialization::make_nvp("m_production_queue", empty_production_queue)
            & boost::serialization::make_nvp("m_influence_queue", empty_influence_queue)
            & boost::serialization::make_nvp("m_available_building_types", empty_string_set)
            & boost::serialization::make_nvp("m_available_part_types", empty_string_set)
            & boost::serialization::make_nvp("m_available_hull_types", empty_string_set);
    } else {
        // processing all data on deserialization, saving to savegame,
        // or sending data to the current empire itself
        ar  & BOOST_SERIALIZATION_NVP(m_research_queue)
            & BOOST_SERIALIZATION_NVP(m_research_progress)
            & BOOST_SERIALIZATION_NVP(m_production_queue)
            & BOOST_SERIALIZATION_NVP(m_influence_queue)
            & BOOST_SERIALIZATION_NVP(m_available_building_types)
            & boost::serialization::make_nvp("m_available_part_types", m_available_ship_parts)
            & boost::serialization::make_nvp("m_available_hull_types", m_available_ship_hulls);
    }

    ar  & BOOST_SERIALIZATION_NVP(m_supply_system_ranges)
        & BOOST_SERIALIZATION_NVP(m_supply_unobstructed_systems)
        & BOOST_SERIALIZATION_NVP(m_preserved_system_exit_lanes);

    if (visible) {
        ar  & boost::serialization::make_nvp("m_ship_designs", m_known_ship_designs);
        ar  & BOOST_SERIALIZATION_NVP(m_sitrep_entries)
            & BOOST_SERIALIZATION_NVP(m_resource_pools)
            & BOOST_SERIALIZATION_NVP(m_population_pool)

            & BOOST_SERIALIZATION_NVP(m_explored_systems)
            & BOOST_SERIALIZATION_NVP(m_ship_names_used)

            & BOOST_SERIALIZATION_NVP(m_species_ships_owned)
            & BOOST_SERIALIZATION_NVP(m_ship_designs_owned)
            & boost::serialization::make_nvp("m_ship_part_types_owned", m_ship_parts_owned)
            & BOOST_SERIALIZATION_NVP(m_ship_part_class_owned)
            & BOOST_SERIALIZATION_NVP(m_species_colonies_owned)
            & BOOST_SERIALIZATION_NVP(m_outposts_owned)
            & BOOST_SERIALIZATION_NVP(m_building_types_owned)

            & BOOST_SERIALIZATION_NVP(m_empire_ships_destroyed)
            & BOOST_SERIALIZATION_NVP(m_ship_designs_destroyed)
            & BOOST_SERIALIZATION_NVP(m_species_ships_destroyed)
            & BOOST_SERIALIZATION_NVP(m_species_planets_invaded)

            & BOOST_SERIALIZATION_NVP(m_ship_designs_in_production)
            & BOOST_SERIALIZATION_NVP(m_species_ships_produced)
            & BOOST_SERIALIZATION_NVP(m_ship_designs_produced)
            & BOOST_SERIALIZATION_NVP(m_species_ships_lost)
            & BOOST_SERIALIZATION_NVP(m_ship_designs_lost)
            & BOOST_SERIALIZATION_NVP(m_species_ships_scrapped)
            & BOOST_SERIALIZATION_NVP(m_ship_designs_scrapped)

            & BOOST_SERIALIZATION_NVP(m_species_planets_depoped)
            & BOOST_SERIALIZATION_NVP(m_species_planets_bombed)

            & BOOST_SERIALIZATION_NVP(m_building_types_produced)
            & BOOST_SERIALIZATION_NVP(m_building_types_scrapped);
    }

    if (Archive::is_loading::value && version < 3) {
        m_authenticated = false;
    } else {
        ar  & BOOST_SERIALIZATION_NVP(m_authenticated);
    }

    if (Archive::is_loading::value && version < 4) {
        m_ready = false;
    } else {
        ar  & BOOST_SERIALIZATION_NVP(m_ready);
    }
}

BOOST_CLASS_VERSION(Empire, 4)

template void Empire::serialize<freeorion_bin_oarchive>(freeorion_bin_oarchive&, const unsigned int);
template void Empire::serialize<freeorion_bin_iarchive>(freeorion_bin_iarchive&, const unsigned int);
template void Empire::serialize<freeorion_xml_oarchive>(freeorion_xml_oarchive&, const unsigned int);
template void Empire::serialize<freeorion_xml_iarchive>(freeorion_xml_iarchive&, const unsigned int);


namespace {
    std::pair<int, int> DiploKey(int id1, int ind2)
    { return std::make_pair(std::max(id1, ind2), std::min(id1, ind2)); }
}

template <typename Archive>
void serialize(Archive& ar, EmpireManager& em, unsigned int const version)
{
    using namespace boost::serialization;

    if (Archive::is_loading::value) {
        em.Clear();    // clean up any existing dynamically allocated contents before replacing containers with deserialized data
    }

    std::map<std::pair<int, int>, DiplomaticMessage> messages;
    if (Archive::is_saving::value)
        em.GetDiplomaticMessagesToSerialize(messages, GetUniverse().EncodingEmpire());

    ar  & make_nvp("m_empire_map", em.m_empire_map)
        & make_nvp("m_empire_diplomatic_statuses", em.m_empire_diplomatic_statuses)
        & BOOST_SERIALIZATION_NVP(messages);

    if (Archive::is_loading::value) {
        em.m_diplomatic_messages = std::move(messages);

        // erase invalid empire diplomatic statuses
        std::vector<std::pair<int, int>> to_erase;
        for (auto r : em.m_empire_diplomatic_statuses) {
            auto e1 = r.first.first;
            auto e2 = r.first.second;
            if (em.m_empire_map.count(e1) < 1 || em.m_empire_map.count(e2) < 1) {
                to_erase.push_back({e1, e2});
                ErrorLogger() << "Erased invalid diplomatic status between empires " << e1 << " and " << e2;
            }
        }
        for (auto p : to_erase)
            em.m_empire_diplomatic_statuses.erase(p);

        // add missing empire diplomatic statuses
        for (auto e1 : em.m_empire_map) {
            for (auto e2 : em.m_empire_map) {
                if (e1.first >= e2.first)
                    continue;
                auto dk = DiploKey(e1.first, e2.first);
                if (em.m_empire_diplomatic_statuses.count(dk) < 1)
                {
                    em.m_empire_diplomatic_statuses[dk] = DIPLO_WAR;
                    ErrorLogger() << "Added missing diplomatic status (default WAR) between empires " << e1.first << " and " << e2.first;
                }
            }
        }
    }
}

template void serialize<freeorion_bin_oarchive>(freeorion_bin_oarchive&, EmpireManager&, unsigned int const);
template void serialize<freeorion_bin_iarchive>(freeorion_bin_iarchive&, EmpireManager&, unsigned int const);
template void serialize<freeorion_xml_oarchive>(freeorion_xml_oarchive&, EmpireManager&, unsigned int const);
template void serialize<freeorion_xml_iarchive>(freeorion_xml_iarchive&, EmpireManager&, unsigned int const);


template <typename Archive>
void serialize(Archive& ar, DiplomaticMessage& obj, unsigned int const version)
{
    using namespace boost::serialization;

    ar  & make_nvp("m_sender_empire", obj.m_sender_empire)
        & make_nvp("m_recipient_empire", obj.m_recipient_empire)
        & make_nvp("m_type", obj.m_type);
}

template void serialize<freeorion_bin_oarchive>(freeorion_bin_oarchive&, DiplomaticMessage&, unsigned int const);
template void serialize<freeorion_bin_iarchive>(freeorion_bin_iarchive&, DiplomaticMessage&, unsigned int const);
template void serialize<freeorion_xml_oarchive>(freeorion_xml_oarchive&, DiplomaticMessage&, unsigned int const);
template void serialize<freeorion_xml_iarchive>(freeorion_xml_iarchive&, DiplomaticMessage&, unsigned int const);


template <typename Archive>
void serialize(Archive& ar, SupplyManager& obj, unsigned int const version)
{
    using namespace boost::serialization;

    ar  & make_nvp("m_supply_starlane_traversals", obj.m_supply_starlane_traversals)
        & make_nvp("m_supply_starlane_obstructed_traversals", obj.m_supply_starlane_obstructed_traversals)
        & make_nvp("m_fleet_supplyable_system_ids", obj.m_fleet_supplyable_system_ids)
        & make_nvp("m_resource_supply_groups", obj.m_resource_supply_groups)
        & make_nvp("m_propagated_supply_ranges", obj.m_propagated_supply_ranges)
        & make_nvp("m_empire_propagated_supply_ranges", obj.m_empire_propagated_supply_ranges)
        & make_nvp("m_propagated_supply_distances", obj.m_propagated_supply_distances)
        & make_nvp("m_empire_propagated_supply_distances", obj.m_empire_propagated_supply_distances);
}

template void serialize<freeorion_bin_oarchive>(freeorion_bin_oarchive&, SupplyManager&, unsigned int const);
template void serialize<freeorion_bin_iarchive>(freeorion_bin_iarchive&, SupplyManager&, unsigned int const);
template void serialize<freeorion_xml_oarchive>(freeorion_xml_oarchive&, SupplyManager&, unsigned int const);
template void serialize<freeorion_xml_iarchive>(freeorion_xml_iarchive&, SupplyManager&, unsigned int const);
