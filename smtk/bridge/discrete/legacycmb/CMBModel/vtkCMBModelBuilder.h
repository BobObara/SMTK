//=========================================================================
//  Copyright (c) Kitware, Inc.
//  All rights reserved.
//  See LICENSE.txt for details.
//
//  This software is distributed WITHOUT ANY WARRANTY; without even
//  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
//  PURPOSE.  See the above copyright notice for more information.
//=========================================================================

// .NAME vtkCMBModelBuilder - A CMB model builder object
// .SECTION Description
// This CMB Model builder takes a vtkPolyData as input, then parse and convert
// all the topology and geometry info from the input to fill in a CMB model.

#ifndef __vtkCMBModelBuilder_h
#define __vtkCMBModelBuilder_h

#include "vtkCmbDiscreteModelModule.h" // For export macro
#include "vtkObject.h"
#include "cmbSystemConfig.h"

class vtkCellLocator;
class vtkDiscreteModelRegion;
class vtkDiscreteModelWrapper;
class vtkPolyData;
class vtkAlgorithm;
class vtkIdList;
class vtkIntArray;

class VTKCMBDISCRETEMODEL_EXPORT vtkCMBModelBuilder : public vtkObject
{
public:
  static vtkCMBModelBuilder * New();
  vtkTypeMacro(vtkCMBModelBuilder,vtkObject);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Load the input polydata into Model.
  void Operate(vtkDiscreteModelWrapper* modelWrapper,
    vtkAlgorithm* inputPoly);

  // Description:
  // Returns success (1) or failue (0) for Operation.
  vtkGetMacro(OperateSucceeded, int);

protected:
  vtkCMBModelBuilder();
  virtual ~vtkCMBModelBuilder();

private:

  // Description:
  // Internal ivars.
  char* FileName;
  vtkCMBModelBuilder(const vtkCMBModelBuilder&);  // Not implemented.
  void operator=(const vtkCMBModelBuilder&);  // Not implemented.

  void ProcessAs2DMesh(vtkDiscreteModelWrapper* ModelWrapper, vtkPolyData *modelPolyData);

  void ComputePointInsideForRegion(vtkDiscreteModelRegion *region,
    vtkCellLocator *locator);

  // Description:
  // Flag to indicate that the operation on the model succeeded (1) or not (0).
  int OperateSucceeded;
};

#endif