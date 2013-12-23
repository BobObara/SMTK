#include "smtk/model/EntityPhrase.h"

#include "smtk/model/CellEntity.h"
#include "smtk/model/EntityListPhrase.h"
#include "smtk/model/GroupEntity.h"
#include "smtk/model/InstanceEntity.h"
#include "smtk/model/ModelEntity.h"
#include "smtk/model/PropertyListPhrase.h"
#include "smtk/model/ShellEntity.h"
#include "smtk/model/UseEntity.h"

namespace smtk {
  namespace model {

EntityPhrase::EntityPhrase()
{
}

/**\brief Prepare an EntityPhrase for display.
  */
EntityPhrase::Ptr EntityPhrase::setup(const Cursor& entity, DescriptivePhrase::Ptr parnt)
{
  this->DescriptivePhrase::setup(ENTITY_SUMMARY, parnt);
  this->m_entity = entity;
  return this->shared_from_this();
}

/// Show the entity name (or a default name) in the title
std::string EntityPhrase::title()
{
  return this->m_entity.name();
}

/// Show the entity type in the subtitle
std::string EntityPhrase::subtitle()
{
  return this->m_entity.flagSummary();
}

/// Return the entity for additional context the UI might wish to present.
Cursor EntityPhrase::relatedEntity() const
{
  return this->m_entity;
}

DescriptivePhrases EntityPhrase::PhrasesFromUUIDs(
  smtk::model::StoragePtr storage, const smtk::util::UUIDs& uids)
{
  DescriptivePhrases result;
  smtk::util::UUIDs::const_iterator it;
  for (it = uids.begin(); it != uids.end(); ++it)
    {
    result.push_back(
      EntityPhrase::create()->setup(Cursor(storage, *it)));
    }
  return result;
}

bool EntityPhrase::buildSubphrasesInternal()
{
  // I. Determine dimension of parent.
  //    We will avoid reporting sub-entities if this entity has a
  //    dimension higher than its parent.
  int dimBits = 0;
  for (DescriptivePhrasePtr phr = this->parent(); phr; phr = phr->parent())
    {
    Cursor c(phr->relatedEntity());
    if (c.isValid() && c.dimensionBits() > 0)
      {
      dimBits = c.dimensionBits();
      break;
      }
    }
  if (dimBits > 0 && (dimBits > this->relatedEntity().dimensionBits() || this->relatedEntity().isModelEntity()))
    {
    return true;
    }
  // II. Add arrangement information
  // This is dependent on both the entity type and the ArrangementKind
  // so we cast to different cursor types and use their accessors to
  // obtain lists of related entities.
   {
   UseEntity uent = this->m_entity.as<UseEntity>();
   CellEntity cent = this->m_entity.as<CellEntity>();
   ShellEntity sent = this->m_entity.as<ShellEntity>();
   GroupEntity gent = this->m_entity.as<GroupEntity>();
   ModelEntity ment = this->m_entity.as<ModelEntity>();
   InstanceEntity ient = this->m_entity.as<InstanceEntity>();
   if (uent.isValid())
     {
     }
   else if (cent.isValid())
     {
     ShellEntities toplevelShells = cent.shells();
     if (!toplevelShells.empty())
       {
       this->m_subphrases.push_back(
         EntityListPhrase::create()->setup(
           toplevelShells, shared_from_this()));
       }
     UseEntities cellUses = cent.uses();
     if (!cellUses.empty())
       {
       this->m_subphrases.push_back(
         EntityListPhrase::create()->setup(
           cellUses, shared_from_this()));
       }
     }
   else if (sent.isValid())
     {
     }
   else if (gent.isValid())
     {
     }
   else if (ment.isValid())
     {
     // TODO: Groups in model
     // TODO: Submodels in model
     CellEntities freeCellsInModel = ment.cells();
     if (!freeCellsInModel.empty())
       {
       this->m_subphrases.push_back(
         EntityListPhrase::create()->setup(
           freeCellsInModel, shared_from_this()));
       }
     }
   else if (ient.isValid())
     {
     }
   }
  // III. Add attribute information
  // TODO.
  // IV. Add property information
  if (this->m_entity.hasStringProperties())
    { // TODO: If m_entity.stringProperties().size() < N, add PropValuePhrases instead of a list.
    PropertyListPhrase::Ptr plist =
      PropertyListPhrase::create()->setup(
        this->m_entity, STRING_PROPERTY, shared_from_this());
    if (!plist->subphrases().empty())
      {
      this->m_subphrases.push_back(plist);
      }
    }
  if (this->m_entity.hasFloatProperties())
    { // TODO: If m_entity.floatProperties().size() < N, add PropValuePhrases instead of a list.
    this->m_subphrases.push_back(
      PropertyListPhrase::create()->setup(
        this->m_entity, FLOAT_PROPERTY, shared_from_this()));
    }
  if (this->m_entity.hasIntegerProperties())
    { // TODO: If m_entity.integerProperties().size() < N, add PropValuePhrases instead of a list.
    this->m_subphrases.push_back(
      PropertyListPhrase::create()->setup(
        this->m_entity, INTEGER_PROPERTY, shared_from_this()));
    }
  return true;
}

  } // model namespace
} // smtk namespace