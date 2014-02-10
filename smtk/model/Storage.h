#ifndef __smtk_model_Storage_h
#define __smtk_model_Storage_h

#include "smtk/model/BRepModel.h"

#include "smtk/PublicPointerDefs.h"
#include "smtk/model/Arrangement.h"
#include "smtk/model/AttributeAssignments.h"
#include "smtk/model/Entity.h"
#include "smtk/model/Tessellation.h"

#include <algorithm>
#include <set>
#include <map>
#include <vector>

#include <sstream>

namespace smtk {
  namespace model {

class Chain;
class Edge;
class EdgeUse;
class Face;
class FaceUse;
class GroupEntity;
class InstanceEntity;
class Loop;
class ModelEntity;
class Shell;
class Vertex;
class VertexUse;
class Volume;
class VolumeUse;

/**\brief Store information about solid models.
  *
  * This adds information about arrangements and tessellations
  * of entities to its BRepModel base class.
  */
class SMTKCORE_EXPORT Storage : public BRepModel
{
public:
  typedef UUIDsToTessellations::iterator tess_iter_type;

  smtkTypeMacro(Storage);
  smtkCreateMacro(Storage);
  smtkSharedFromThisMacro(BRepModel);
  Storage();
  Storage(
    shared_ptr<UUIDsToEntities> topology,
    shared_ptr<UUIDsToArrangements> arrangements,
    shared_ptr<UUIDsToTessellations> tess,
    shared_ptr<UUIDsToAttributeAssignments> attribs);
  virtual ~Storage();

  UUIDsToArrangements& arrangements();
  const UUIDsToArrangements& arrangements() const;

  UUIDsToTessellations& tessellations();
  const UUIDsToTessellations& tessellations() const;

  UUIDsToAttributeAssignments& attributeAssignments();
  const UUIDsToAttributeAssignments& attributeAssignments() const;

  smtk::attribute::Manager* attributeManager() const;

  virtual bool erase(const smtk::util::UUID& uid);

  tess_iter_type setTessellation(const smtk::util::UUID& cellId, const Tessellation& geom);

  int arrangeEntity(const smtk::util::UUID& entityId, ArrangementKind, const Arrangement& arr, int index = -1);
  int unarrangeEntity(const smtk::util::UUID& entityId, ArrangementKind, int index, bool removeIfLast = false);

  const Arrangements* hasArrangementsOfKindForEntity(
    const smtk::util::UUID& cellId,
    ArrangementKind) const;
  Arrangements* hasArrangementsOfKindForEntity(
    const smtk::util::UUID& cellId,
    ArrangementKind);

  Arrangements& arrangementsOfKindForEntity(const smtk::util::UUID& cellId, ArrangementKind);

  const Arrangement* findArrangement(const smtk::util::UUID& entityId, ArrangementKind kind, int index) const;
  Arrangement* findArrangement(const smtk::util::UUID& entityId, ArrangementKind kind, int index);
  int findArrangementInvolvingEntity(
    const smtk::util::UUID& entityId, ArrangementKind kind,
    const smtk::util::UUID& involved) const;
  bool findDualArrangements(
    const smtk::util::UUID& entityId, ArrangementKind kind, int index,
    ArrangementReferences& duals) const;

  int findCellHasUseWithSense(const smtk::util::UUID& cellId, int sense) const;
  std::set<int> findCellHasUsesWithOrientation(const smtk::util::UUID& cellId, Orientation orient) const;

  smtk::util::UUID cellHasUseOfSenseAndOrientation(const smtk::util::UUID& cell, int sense, Orientation o) const;
  smtk::util::UUID findCreateOrReplaceCellUseOfSenseAndOrientation(
    const smtk::util::UUID& cell, int sense, Orientation o,
    const smtk::util::UUID& replacement = smtk::util::UUID::null());

  smtk::util::UUIDs useOrShellIncludesShells(const smtk::util::UUID& cellUseOrShell) const;
  smtk::util::UUID createIncludedShell(const smtk::util::UUID& cellUseOrShell);

  //bool shellHasUse(const smtk::util::UUID& shell, const smtk::util::UUID& use) const;
  //smtk::util::UUIDs shellHasUses(const smtk::util::UUID& shell) const;
  bool findOrAddUseToShell(const smtk::util::UUID& shell, const smtk::util::UUID& use);

  bool findOrAddInclusionToCell(const smtk::util::UUID& cell, const smtk::util::UUID& inclusion);

  bool findOrAddEntityToGroup(const smtk::util::UUID& grp, const smtk::util::UUID& ent);

  bool hasAttribute(int attribId, const smtk::util::UUID& toEntity);
  bool attachAttribute(int attribId, const smtk::util::UUID& toEntity);
  bool detachAttribute(int attribId, const smtk::util::UUID& fromEntity, bool reverse = true);

  Vertex insertVertex(const smtk::util::UUID& uid);
  Edge insertEdge(const smtk::util::UUID& uid);
  Face insertFace(const smtk::util::UUID& uid);
  Volume insertVolume(const smtk::util::UUID& uid);

  Vertex addVertex();
  Edge addEdge();
  Face addFace();
  Volume addVolume();

  /*
  VertexUse insertVertexUse(const smtk::util::UUID& uid);
  VertexUse insertVertexUse(const smtk::util::UUID& uid, const Vertex& src, int sense);
  EdgeUse insertEdgeUse(const smtk::util::UUID& uid);
  EdgeUse insertEdgeUse(const smtk::util::UUID& uid, const Edge& src, int sense, Orientation o);
  FaceUse insertFaceUse(const smtk::util::UUID& uid);
  FaceUse insertFaceUse(const smtk::util::UUID& uid, const Face& src, int sense, Orientation o);
  */
  VolumeUse insertVolumeUse(const smtk::util::UUID& uid);
  VolumeUse setVolumeUse(const smtk::util::UUID& uid, const Volume& src);

  VertexUse addVertexUse();
  VertexUse addVertexUse(const Vertex& src, int sense);
  EdgeUse addEdgeUse();
  EdgeUse addEdgeUse(const Edge& src, int sense, Orientation o);
  FaceUse addFaceUse();
  FaceUse addFaceUse(const Face& src, int sense, Orientation o);
  VolumeUse addVolumeUse();
  VolumeUse addVolumeUse(const Volume& src);

  Chain addChain();
  Chain addChain(const EdgeUse&);
  Chain addChain(const Chain&);
  Loop addLoop();
  Loop addLoop(const FaceUse&);
  Loop addLoop(const Loop&);
  Shell addShell();
  Shell addShell(const Volume& src);
  Shell addShell(const VolumeUse& src);

  GroupEntity insertGroup(
    const smtk::util::UUID& uid,
    int extraFlags = 0,
    const std::string& name = std::string());
  GroupEntity addGroup(int extraFlags = 0, const std::string& name = std::string());

  ModelEntity insertModel(
    const smtk::util::UUID& uid,
    int parametricDim = 3,
    int embeddingDim = 3,
    const std::string& name = std::string());
  ModelEntity addModel(
    int parametricDim = 3, int embeddingDim = 3, const std::string& name = std::string());

  InstanceEntity addInstance();
  InstanceEntity addInstance(const Cursor& instanceOf);

protected:
  friend class smtk::attribute::Manager;
  bool setAttributeManager(smtk::attribute::Manager* mgr, bool reverse = true);

  shared_ptr<UUIDsToArrangements> m_arrangements;
  shared_ptr<UUIDsToTessellations> m_tessellations;
  shared_ptr<UUIDsToAttributeAssignments> m_attributeAssignments;
  smtk::attribute::Manager* m_attributeManager;
};

  } // model namespace
} // smtk namespace

#endif // __smtk_model_Storage_h
