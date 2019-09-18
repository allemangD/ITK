/*=========================================================================
 *
 *  Copyright Insight Software Consortium
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/
#include "itkTestingExtractSliceImageFilter.h"

namespace itk
{
namespace Testing
{
/** Define how to print enumerations */
std::ostream &
operator<<(std::ostream & out, const TestExtractSliceImageFilterCollapseStrategy value)
{
  return out << [value] {
    switch (value)
    {
      case TestExtractSliceImageFilterCollapseStrategy::DIRECTIONCOLLAPSETOUNKOWN:
        return "TestExtractSliceImageFilterCollapseStrategy::DIRECTIONCOLLAPSETOUNKOWN";
      case TestExtractSliceImageFilterCollapseStrategy::DIRECTIONCOLLAPSETOIDENTITY:
        return "TestExtractSliceImageFilterCollapseStrategy::DIRECTIONCOLLAPSETOIDENTITY";
      case TestExtractSliceImageFilterCollapseStrategy::DIRECTIONCOLLAPSETOSUBMATRIX:
        return "TestExtractSliceImageFilterCollapseStrategy::DIRECTIONCOLLAPSETOSUBMATRIX";
      case TestExtractSliceImageFilterCollapseStrategy::DIRECTIONCOLLAPSETOGUESS:
        return "TestExtractSliceImageFilterCollapseStrategy::DIRECTIONCOLLAPSETOGUESS";
      default:
        return "INVALID VALUE FOR TestExtractSliceImageFilterCollapseStrategy";
    }
  }();
}
} // end namespace Testing
} // end namespace itk
