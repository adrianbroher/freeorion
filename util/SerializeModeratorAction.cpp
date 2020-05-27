#include "Serialize.h"

#include "ModeratorAction.h"

#include "Serialize.ipp"


BOOST_CLASS_EXPORT(Moderator::DestroyUniverseObject)
BOOST_CLASS_EXPORT(Moderator::SetOwner)
BOOST_CLASS_EXPORT(Moderator::AddStarlane)
BOOST_CLASS_EXPORT(Moderator::RemoveStarlane)
BOOST_CLASS_EXPORT(Moderator::CreateSystem)
BOOST_CLASS_EXPORT(Moderator::CreatePlanet)


template <typename Archive>
void Moderator::serialize(Archive&, ModeratorAction&, unsigned int const)
{}

template void Moderator::serialize<freeorion_bin_oarchive>(freeorion_bin_oarchive&, ModeratorAction&, unsigned int const);
template void Moderator::serialize<freeorion_bin_iarchive>(freeorion_bin_iarchive&, ModeratorAction&, unsigned int const);
template void Moderator::serialize<freeorion_xml_oarchive>(freeorion_xml_oarchive&, ModeratorAction&, unsigned int const);
template void Moderator::serialize<freeorion_xml_iarchive>(freeorion_xml_iarchive&, ModeratorAction&, unsigned int const);

template <typename Archive>
void Moderator::serialize(Archive& ar, DestroyUniverseObject& obj, unsigned int const)
{
    using namespace boost::serialization;

    ar  & make_nvp("ModeratorAction", base_object<ModeratorAction>(obj))
        & make_nvp("m_object_id", obj.m_object_id);
}

template void Moderator::serialize<freeorion_bin_oarchive>(freeorion_bin_oarchive&, DestroyUniverseObject&, const unsigned int);
template void Moderator::serialize<freeorion_bin_iarchive>(freeorion_bin_iarchive&, DestroyUniverseObject&, const unsigned int);
template void Moderator::serialize<freeorion_xml_oarchive>(freeorion_xml_oarchive&, DestroyUniverseObject&, const unsigned int);
template void Moderator::serialize<freeorion_xml_iarchive>(freeorion_xml_iarchive&, DestroyUniverseObject&, const unsigned int);

template <typename Archive>
void Moderator::serialize(Archive& ar, SetOwner& obj, unsigned int const)
{
    using namespace boost::serialization;

    ar  & make_nvp("ModeratorAction", base_object<ModeratorAction>(obj))
        & make_nvp("m_object_id", obj.m_object_id)
        & make_nvp("m_new_owner_empire_id", obj.m_new_owner_empire_id);
}

template void Moderator::serialize<freeorion_bin_oarchive>(freeorion_bin_oarchive&, SetOwner&, const unsigned int);
template void Moderator::serialize<freeorion_bin_iarchive>(freeorion_bin_iarchive&, SetOwner&, const unsigned int);
template void Moderator::serialize<freeorion_xml_oarchive>(freeorion_xml_oarchive&, SetOwner&, const unsigned int);
template void Moderator::serialize<freeorion_xml_iarchive>(freeorion_xml_iarchive&, SetOwner&, const unsigned int);

template <typename Archive>
void Moderator::serialize(Archive& ar, AddStarlane& obj, unsigned int const)
{
    using namespace boost::serialization;

    ar  & make_nvp("ModeratorAction", base_object<ModeratorAction>(obj))
        & make_nvp("m_id_1", obj.m_id_1)
        & make_nvp("m_id_2", obj.m_id_2);
}

template void Moderator::serialize<freeorion_bin_oarchive>(freeorion_bin_oarchive&, AddStarlane&, const unsigned int);
template void Moderator::serialize<freeorion_bin_iarchive>(freeorion_bin_iarchive&, AddStarlane&, const unsigned int);
template void Moderator::serialize<freeorion_xml_oarchive>(freeorion_xml_oarchive&, AddStarlane&, const unsigned int);
template void Moderator::serialize<freeorion_xml_iarchive>(freeorion_xml_iarchive&, AddStarlane&, const unsigned int);

template <typename Archive>
void Moderator::serialize(Archive& ar, RemoveStarlane& obj, unsigned int const)
{
    using namespace boost::serialization;

    ar  & make_nvp("ModeratorAction", base_object<ModeratorAction>(obj))
        & make_nvp("m_id_1", obj.m_id_1)
        & make_nvp("m_id_2", obj.m_id_2);
}

template void Moderator::serialize<freeorion_bin_oarchive>(freeorion_bin_oarchive&, RemoveStarlane&, const unsigned int);
template void Moderator::serialize<freeorion_bin_iarchive>(freeorion_bin_iarchive&, RemoveStarlane&, const unsigned int);
template void Moderator::serialize<freeorion_xml_oarchive>(freeorion_xml_oarchive&, RemoveStarlane&, const unsigned int);
template void Moderator::serialize<freeorion_xml_iarchive>(freeorion_xml_iarchive&, RemoveStarlane&, const unsigned int);

template <typename Archive>
void Moderator::serialize(Archive& ar, CreateSystem& obj, unsigned int const)
{
    using namespace boost::serialization;

    ar  & make_nvp("ModeratorAction", base_object<ModeratorAction>(obj))
        & make_nvp("m_x", obj.m_x)
        & make_nvp("m_y", obj.m_y)
        & make_nvp("m_star_type", obj.m_star_type);
}

template void Moderator::serialize<freeorion_bin_oarchive>(freeorion_bin_oarchive&, CreateSystem& obj, const unsigned int);
template void Moderator::serialize<freeorion_bin_iarchive>(freeorion_bin_iarchive&, CreateSystem& obj, const unsigned int);
template void Moderator::serialize<freeorion_xml_oarchive>(freeorion_xml_oarchive&, CreateSystem& obj, const unsigned int);
template void Moderator::serialize<freeorion_xml_iarchive>(freeorion_xml_iarchive&, CreateSystem& obj, const unsigned int);

template <typename Archive>
void Moderator::serialize(Archive& ar, CreatePlanet& obj, unsigned int const)
{
    using namespace boost::serialization;

    ar  & make_nvp("ModeratorAction", base_object<ModeratorAction>(obj))
        & make_nvp("m_system_id", obj.m_system_id)
        & make_nvp("m_planet_type", obj.m_planet_type)
        & make_nvp("m_planet_size", obj.m_planet_size);
}

template void Moderator::serialize<freeorion_bin_oarchive>(freeorion_bin_oarchive&, CreatePlanet&, const unsigned int);
template void Moderator::serialize<freeorion_bin_iarchive>(freeorion_bin_iarchive&, CreatePlanet&, const unsigned int);
template void Moderator::serialize<freeorion_xml_oarchive>(freeorion_xml_oarchive&, CreatePlanet&, const unsigned int);
template void Moderator::serialize<freeorion_xml_iarchive>(freeorion_xml_iarchive&, CreatePlanet&, const unsigned int);
