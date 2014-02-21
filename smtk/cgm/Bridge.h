#ifndef __smtk_cgm_Bridge_h
#define __smtk_cgm_Bridge_h

#include "smtk/cgmSMTKExports.h"
#include "smtk/model/BridgeBase.h"

class Body;
class CoVolume;
class CoFace;
class CoEdge;
class CoVertex;
class Shell;
class Loop;
class Chain;
class RefVolume;
class RefFace;
class RefEdge;
class RefVertex;
class RefGroup;
class RefEntity;
class SenseEntity;
class GroupingEntity;

namespace cgmsmtk {
  namespace cgm {

/**\brief Methods that handle translation between CGM and SMTK instances.
  *
  * While the TDUUID class keeps a map from SMTK UUIDs to CGM ToolDataUser
  * pointers, this is not enough to handle everything SMTK provides:
  * there is no way to track cell-use or shell entities since they do
  * not inherit ToolDataUser instances. Also, some engines (e.g., facet)
  * do not appear to store some entity types (e.g., RefGroup).
  *
  * Also, simply loading a CGM file does not translate the entire model
  * into SMTK; instead, it assigns UUIDs to entities if they do not already
  * exist. This class (Bridge) provides a method for requesting the
  * entity, arrangement, and/or tessellation information for a UUID be
  * mapped into SMTK from CGM.
  */
class CGMSMTK_EXPORT Bridge : public smtk::model::BridgeBase
{
public:
  typedef smtk::shared_ptr<Bridge> Ptr;
  typedef smtk::model::BridgedInfoBits BridgedInfoBits;
  static BridgePtr create();
  virtual ~Bridge();

  virtual BridgedInfoBits allSupportedInformation() const;

  static bool addStorageEntityToCGM(const smtk::model::Cursor& ent);

protected:
  friend class ImportSolid;

  Bridge();

  virtual BridgedInfoBits transcribeInternal(
    const smtk::model::Cursor& entity, BridgedInfoBits requestedInfo);

  BridgedInfoBits addCGMEntityToStorage(const smtk::model::Cursor& entity, RefEntity* refEnt, BridgedInfoBits requestedInfo);
  BridgedInfoBits addCGMEntityToStorage(const smtk::model::Cursor& entity, GroupingEntity* refEnt, BridgedInfoBits requestedInfo);
  BridgedInfoBits addCGMEntityToStorage(const smtk::model::Cursor& entity, SenseEntity* refEnt, BridgedInfoBits requestedInfo);

  BridgedInfoBits addBodyToStorage(const smtk::model::ModelEntity&, Body*, BridgedInfoBits requestedInfo);
  BridgedInfoBits addVolumeUseToStorage(const smtk::model::VolumeUse&, CoVolume*, BridgedInfoBits requestedInfo);
  BridgedInfoBits addFaceUseToStorage(const smtk::model::FaceUse&, CoFace*, BridgedInfoBits requestedInfo);
  BridgedInfoBits addEdgeUseToStorage(const smtk::model::EdgeUse&, CoEdge*, BridgedInfoBits requestedInfo);
  BridgedInfoBits addVertexUseToStorage(const smtk::model::VertexUse&, CoVertex*, BridgedInfoBits requestedInfo);
  BridgedInfoBits addShellToStorage(const smtk::model::Shell&, ::Shell*, BridgedInfoBits requestedInfo);
  BridgedInfoBits addLoopToStorage(const smtk::model::Loop&, ::Loop*, BridgedInfoBits requestedInfo);
  BridgedInfoBits addChainToStorage(const smtk::model::Chain&, ::Chain*, BridgedInfoBits requestedInfo);
  BridgedInfoBits addVolumeToStorage(const smtk::model::Volume&, RefVolume*, BridgedInfoBits requestedInfo);
  BridgedInfoBits addFaceToStorage(const smtk::model::Face&, RefFace*, BridgedInfoBits requestedInfo);
  BridgedInfoBits addEdgeToStorage(const smtk::model::Edge&, RefEdge*, BridgedInfoBits requestedInfo);
  BridgedInfoBits addVertexToStorage(const smtk::model::Vertex&, RefVertex*, BridgedInfoBits requestedInfo);
  BridgedInfoBits addGroupToStorage(const smtk::model::GroupEntity&, RefGroup*, BridgedInfoBits requestedInfo);

  static void colorPropFromIndex(smtk::model::Cursor&, int);

private:
  Bridge(const Bridge&); // Not implemented.
  void operator = (const Bridge&); // Not implemented.
};

  } // namespace cgm
} // namespace cgmsmtk

#endif // __smtk_cgm_Bridge_h