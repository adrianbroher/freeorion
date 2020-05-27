#include "Serialize.h"

#include "Order.h"
#include "OrderSet.h"

#include "Serialize.ipp"
#include <boost/serialization/version.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/nil_generator.hpp>
#include "Logger.h"


BOOST_CLASS_EXPORT(Order)
BOOST_CLASS_VERSION(Order, 1)
BOOST_CLASS_EXPORT(RenameOrder)
BOOST_CLASS_EXPORT(NewFleetOrder)
BOOST_CLASS_VERSION(NewFleetOrder, 1)
BOOST_CLASS_EXPORT(FleetMoveOrder)
BOOST_CLASS_VERSION(FleetMoveOrder, 2)
BOOST_CLASS_EXPORT(FleetTransferOrder)
BOOST_CLASS_EXPORT(ColonizeOrder)
BOOST_CLASS_EXPORT(InvadeOrder)
BOOST_CLASS_EXPORT(BombardOrder)
BOOST_CLASS_EXPORT(ChangeFocusOrder)
BOOST_CLASS_EXPORT(PolicyOrder)
BOOST_CLASS_EXPORT(ResearchQueueOrder)
BOOST_CLASS_EXPORT(ProductionQueueOrder)
BOOST_CLASS_VERSION(ProductionQueueOrder, 2)
BOOST_CLASS_EXPORT(ShipDesignOrder)
BOOST_CLASS_VERSION(ShipDesignOrder, 1)
BOOST_CLASS_EXPORT(ScrapOrder)
BOOST_CLASS_EXPORT(AggressiveOrder)
BOOST_CLASS_EXPORT(GiveObjectToEmpireOrder)
BOOST_CLASS_EXPORT(ForgetOrder)


template <typename Archive>
void serialize(Archive& ar, Order& obj, unsigned int const version)
{
    using namespace boost::serialization;

    ar  & make_nvp("m_empire", obj.m_empire);
    // m_executed is intentionally not serialized so that orders always
    // deserialize with m_execute = false.  See class comment for OrderSet.
    if (Archive::is_loading::value && version < 1) {
        bool dummy_executed;
        ar  & boost::serialization::make_nvp("m_executed", dummy_executed);
    }
}


template <typename Archive>
void load_construct_data(Archive& ar, RenameOrder* obj, unsigned int const version)
{ ::new(obj)RenameOrder(ALL_EMPIRES, INVALID_OBJECT_ID, ""); }

template <typename Archive>
void serialize(Archive& ar, RenameOrder& obj, unsigned int const version)
{
    using namespace boost::serialization;

    ar  & make_nvp("Order", base_object<Order>(obj))
        & make_nvp("m_object", obj.m_object)
        & make_nvp("m_name", obj.m_name);
}

template <typename Archive>
void load_construct_data(Archive& ar, NewFleetOrder* obj, unsigned int const version)
{ ::new(obj)NewFleetOrder(ALL_EMPIRES, "", {}, false); }

template <typename Archive>
void serialize(Archive& ar, NewFleetOrder& obj, unsigned int const version)
{
    using namespace boost::serialization;

    ar  & make_nvp("Order", base_object<Order>(obj))
        & make_nvp("m_fleet_name", obj.m_fleet_name)
        & make_nvp("m_fleet_id", obj.m_fleet_id)
        & make_nvp("m_ship_ids", obj.m_ship_ids)
        & make_nvp("m_aggressive", obj.m_aggressive);
}


template <typename Archive>
void load_construct_data(Archive& ar, FleetMoveOrder* obj, unsigned int const version)
{ ::new(obj)FleetMoveOrder(ALL_EMPIRES, INVALID_OBJECT_ID, INVALID_OBJECT_ID); }

template <typename Archive>
void serialize(Archive& ar, FleetMoveOrder& obj, unsigned int const version)
{
    using namespace boost::serialization;

    ar  & make_nvp("Order", base_object<Order>(obj))
        & make_nvp("m_fleet", obj.m_fleet)
        & make_nvp("m_dest_system", obj.m_dest_system)
        & make_nvp("m_route", obj.m_route);
    if (version > 0) {
        ar & make_nvp("m_append", obj.m_append);
    } else {
        obj.m_append = false;
    }
}


template <typename Archive>
void load_construct_data(Archive& ar, FleetTransferOrder* obj, unsigned int const version)
{ ::new(obj)FleetTransferOrder(ALL_EMPIRES, INVALID_OBJECT_ID, {}); }

template <typename Archive>
void serialize(Archive& ar, FleetTransferOrder& obj, unsigned int const version)
{
    using namespace boost::serialization;

    ar  & make_nvp("Order", base_object<Order>(obj))
        & make_nvp("m_dest_fleet", obj.m_dest_fleet)
        & make_nvp("m_add_ships", obj.m_add_ships);
}


template <typename Archive>
void load_construct_data(Archive& ar, ColonizeOrder* obj, unsigned int const version)
{ ::new(obj)ColonizeOrder(ALL_EMPIRES, INVALID_OBJECT_ID, INVALID_OBJECT_ID); }

template <typename Archive>
void serialize(Archive& ar, ColonizeOrder& obj, unsigned int const version)
{
    using namespace boost::serialization;

    ar  & make_nvp("Order", base_object<Order>(obj))
        & make_nvp("m_ship", obj.m_ship)
        & make_nvp("m_planet", obj.m_planet);
}


template <typename Archive>
void load_construct_data(Archive& ar, InvadeOrder* obj, unsigned int const version)
{ ::new(obj)InvadeOrder(ALL_EMPIRES, INVALID_OBJECT_ID, INVALID_OBJECT_ID); }

template <typename Archive>
void serialize(Archive& ar, InvadeOrder& obj, unsigned int const version)
{
    using namespace boost::serialization;

    ar  & make_nvp("Order", base_object<Order>(obj))
        & make_nvp("m_ship", obj.m_ship)
        & make_nvp("m_planet", obj.m_planet);
}


template <typename Archive>
void load_construct_data(Archive& ar, BombardOrder* obj, unsigned int const version)
{ ::new(obj)BombardOrder(ALL_EMPIRES, INVALID_OBJECT_ID, INVALID_OBJECT_ID); }

template <typename Archive>
void serialize(Archive& ar, BombardOrder& obj, unsigned int const version)
{
    using namespace boost::serialization;

    ar  & make_nvp("Order", base_object<Order>(obj))
        & make_nvp("m_ship", obj.m_ship)
        & make_nvp("m_planet", obj.m_planet);
}


template <typename Archive>
void load_construct_data(Archive& ar, ChangeFocusOrder* obj, unsigned int const version)
{ ::new(obj)ChangeFocusOrder(ALL_EMPIRES, INVALID_OBJECT_ID, ""); }

template <typename Archive>
void serialize(Archive& ar, ChangeFocusOrder& obj, unsigned int const version)
{
    using namespace boost::serialization;

    ar  & make_nvp("Order", base_object<Order>(obj))
        & make_nvp("m_planet", obj.m_planet)
        & make_nvp("m_focus", obj.m_focus);
}


template <typename Archive>
void load_construct_data(Archive& ar, PolicyOrder* obj, unsigned int const version)
{ ::new(obj)PolicyOrder(ALL_EMPIRES, "", "", true); }

template <typename Archive>
void serialize(Archive& ar, PolicyOrder& obj, unsigned int const version)
{
    using namespace boost::serialization;

    ar  & make_nvp("Order", base_object<Order>(obj))
        & make_nvp("m_policy_name", obj.m_policy_name)
        & make_nvp("m_category", obj.m_category)
        & make_nvp("m_adopt", obj.m_adopt)
        & make_nvp("m_slot", obj.m_slot);
}


template <typename Archive>
void load_construct_data(Archive& ar, ResearchQueueOrder* obj, unsigned int const version)
{ ::new(obj)ResearchQueueOrder(ALL_EMPIRES, ""); }

template <class Archive>
void serialize(Archive& ar, ResearchQueueOrder& obj, unsigned int const version)
{
    using namespace boost::serialization;

    ar  & make_nvp("Order", base_object<Order>(obj))
        & make_nvp("m_tech_name", obj.m_tech_name)
        & make_nvp("m_position", obj.m_position)
        & make_nvp("m_remove", obj.m_remove)
        & make_nvp("m_pause", obj.m_pause);
}


template <typename Archive>
void load_construct_data(Archive& ar, ProductionQueueOrder* obj, unsigned int const version)
{ ::new(obj)ProductionQueueOrder(ProductionQueueOrder::INVALID_PROD_QUEUE_ACTION, ALL_EMPIRES, boost::uuids::nil_generator()()); }

template <typename Archive>
void serialize(Archive& ar, ProductionQueueOrder& obj, unsigned int const version)
{
    using namespace boost::serialization;

    ar  & make_nvp("Order", base_object<Order>(obj))
        & make_nvp("m_item", obj.m_item);

    int m_number, m_index, m_pause, m_split_incomplete, m_dupe, m_use_imperial_pp;
    if (version < 2)
        ar  & make_nvp("m_number", m_number);

    ar  & make_nvp("m_location", obj.m_location);

    if (version < 2)
        ar  & make_nvp("m_index", m_index);

    ar  & make_nvp("m_new_quantity", obj.m_new_quantity)
        & make_nvp("m_new_blocksize", obj.m_new_blocksize)
        & make_nvp("m_new_index", obj.m_new_index)
        & make_nvp("m_rally_point_id", obj.m_rally_point_id);

    if (version < 2) {
        ar  & make_nvp("m_pause", m_pause)
            & make_nvp("m_split_incomplete", m_split_incomplete)
            & make_nvp("m_dupe", m_dupe)
            & make_nvp("m_use_imperial_pp", m_use_imperial_pp);
    } else {
        ar  & make_nvp("m_action", obj.m_action);
    }

    if (version < 2 && Archive::is_loading::value) {
        // would need to generate action and UUID from deserialized values. instead generate an invalid order. will break partial-turn saves.
        obj.m_uuid = boost::uuids::nil_generator()();
        obj.m_uuid2 = boost::uuids::nil_generator()();
        obj.m_action = ProductionQueueOrder::INVALID_PROD_QUEUE_ACTION;

    } else {
        // Serialization of m_uuid as a primitive doesn't work as expected from
        // the documentation.  This workaround instead serializes a string
        // representation.
        if (Archive::is_saving::value) {
            auto string_uuid = boost::uuids::to_string(obj.m_uuid);
            ar & make_nvp("string_uuid", string_uuid);
            auto string_uuid2 = boost::uuids::to_string(obj.m_uuid2);
            ar & make_nvp("string_uuid2", string_uuid2);

        } else {
            std::string string_uuid;
            ar & make_nvp("string_uuid", string_uuid);
            std::string string_uuid2;
            ar & make_nvp("string_uuid2", string_uuid2);
            try {
                obj.m_uuid = boost::lexical_cast<boost::uuids::uuid>(string_uuid);
                obj.m_uuid2 = boost::lexical_cast<boost::uuids::uuid>(string_uuid2);
            } catch (const boost::bad_lexical_cast&) {
                ErrorLogger() << "Error casting to UUIDs from strings: " << string_uuid
                              << " and " << string_uuid2 << ".  ProductionOrder will be invalid";
                obj.m_uuid = boost::uuids::nil_generator()();
                obj.m_uuid2 = boost::uuids::nil_generator()();
                obj.m_action = INVALID_PROD_QUEUE_ACTION;
            }
        }
    }
}


template <typename Archive>
void load_construct_data(Archive& ar, ShipDesignOrder* obj, unsigned int const version)
{ ::new(obj)ShipDesignOrder(ALL_EMPIRES, INVALID_DESIGN_ID); }

template <typename Archive>
void serialize(Archive& ar, ShipDesignOrder& obj, unsigned int const version)
{
    using namespace boost::serialization;

    ar  & make_nvp("Order", base_object<Order>(obj));
    ar  & make_nvp("m_design_id", obj.m_design_id);

    if (version >= 1) {
        // Serialization of m_uuid as a primitive doesn't work as expected from
        // the documentation.  This workaround instead serializes a string
        // representation.
        if (Archive::is_saving::value) {
            auto string_uuid = boost::uuids::to_string(obj.m_uuid);
            ar & make_nvp("string_uuid", string_uuid);
        } else {
            std::string string_uuid;
            ar & make_nvp("string_uuid", string_uuid);
            try {
                obj.m_uuid = boost::lexical_cast<boost::uuids::uuid>(string_uuid);
            } catch (const boost::bad_lexical_cast&) {
                obj.m_uuid = boost::uuids::nil_generator()();
            }
        }
    } else if (Archive::is_loading::value) {
        obj.m_uuid = boost::uuids::nil_generator()();
    }

    ar  & make_nvp("m_delete_design_from_empire", obj.m_delete_design_from_empire);
    ar  & make_nvp("m_create_new_design", obj.m_create_new_design);
    ar  & make_nvp("m_update_name_or_description", obj.m_update_name_or_description);
    ar  & make_nvp("m_name", obj.m_name);
    ar  & make_nvp("m_description", obj.m_description);
    ar  & make_nvp("m_designed_on_turn", obj.m_designed_on_turn);
    ar  & make_nvp("m_hull", obj.m_hull);
    ar  & make_nvp("m_parts", obj.m_parts);
    ar  & make_nvp("m_is_monster", obj.m_is_monster);
    ar  & make_nvp("m_icon", obj.m_icon);
    ar  & make_nvp("m_3D_model", obj.m_3D_model);
    ar  & make_nvp("m_name_desc_in_stringtable", obj.m_name_desc_in_stringtable);
}


template <typename Archive>
void load_construct_data(Archive& ar, ScrapOrder* obj, unsigned int const version)
{ ::new(obj)ScrapOrder(ALL_EMPIRES, INVALID_OBJECT_ID); }

template <typename Archive>
void serialize(Archive& ar, ScrapOrder& obj, unsigned int const version)
{
    using namespace boost::serialization;

    ar  & make_nvp("Order", base_object<Order>(obj))
        & make_nvp("m_object_id", obj.m_object_id);
}


template <typename Archive>
void load_construct_data(Archive& ar, AggressiveOrder* obj, unsigned int const version)
{ ::new(obj)AggressiveOrder(ALL_EMPIRES, INVALID_OBJECT_ID); }

template <typename Archive>
void serialize(Archive& ar, AggressiveOrder& obj, unsigned int const version)
{
    using namespace boost::serialization;

    ar  & make_nvp("Order", base_object<Order>(obj))
        & make_nvp("m_object_id", obj.m_object_id)
        & make_nvp("m_aggression", obj.m_aggression);
}


template <typename Archive>
void load_construct_data(Archive& ar, GiveObjectToEmpireOrder* obj, unsigned int const version)
{ ::new(obj)GiveObjectToEmpireOrder(ALL_EMPIRES, INVALID_OBJECT_ID, ALL_EMPIRES); }

template <typename Archive>
void serialize(Archive& ar, GiveObjectToEmpireOrder& obj, unsigned int const version)
{
    using namespace boost::serialization;

    ar  & make_nvp("Order", base_object<Order>(obj))
        & make_nvp("m_object_id", obj.m_object_id)
        & make_nvp("m_recipient_empire_id", obj.m_recipient_empire_id);
}


template <typename Archive>
void load_construct_data(Archive& ar, ForgetOrder* obj, unsigned int const version)
{ ::new(obj)ForgetOrder(ALL_EMPIRES, INVALID_OBJECT_ID); }

template <typename Archive>
void serialize(Archive& ar, ForgetOrder& obj, unsigned int const version)
{
    using namespace boost::serialization;

    ar  & make_nvp("Order", base_object<Order>(obj))
        & make_nvp("m_object_id", obj.m_object_id);
}


template <typename Archive>
void serialize(Archive& ar, OrderSet& obj, unsigned int const version)
{
    ar  & boost::serialization::make_nvp("m_orders", obj.m_orders);
    if (Archive::is_loading::value) {
        obj.m_last_added_orders.clear();
        obj.m_last_deleted_orders.clear();
    }
}

template void serialize<freeorion_bin_oarchive>(freeorion_bin_oarchive&, OrderSet&, unsigned int const);
template void serialize<freeorion_bin_iarchive>(freeorion_bin_iarchive&, OrderSet&, unsigned int const);
template void serialize<freeorion_xml_oarchive>(freeorion_xml_oarchive&, OrderSet&, unsigned int const);
template void serialize<freeorion_xml_iarchive>(freeorion_xml_iarchive&, OrderSet&, unsigned int const);


template <typename Archive>
void Serialize(Archive& oa, const OrderSet& order_set)
{ oa << BOOST_SERIALIZATION_NVP(order_set); }
template void Serialize<freeorion_bin_oarchive>(freeorion_bin_oarchive& oa, const OrderSet& order_set);
template void Serialize<freeorion_xml_oarchive>(freeorion_xml_oarchive& oa, const OrderSet& order_set);

template <typename Archive>
void Deserialize(Archive& ia, OrderSet& order_set)
{ ia >> BOOST_SERIALIZATION_NVP(order_set); }
template void Deserialize<freeorion_bin_iarchive>(freeorion_bin_iarchive& ia, OrderSet& order_set);
template void Deserialize<freeorion_xml_iarchive>(freeorion_xml_iarchive& ia, OrderSet& order_set);
