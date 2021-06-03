/*=========================================================================
 *
 *  Copyright NumFOCUS
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
#ifndef itkBayesianClassifierImageFilter_h
#define itkBayesianClassifierImageFilter_h

#include "itkVectorImage.h"
#include "itkImageToImageFilter.h"
#include "itkMaximumDecisionRule.h"
#include "itkImageRegionIterator.h"

namespace itk
{
/**
 *\class BayesianClassifierImageFilter
 *
 * \brief Performs Bayesian Classification on an image.
 *
 * \par Inputs and Outputs
 * The input to this filter is an itk::VectorImage that represents pixel
 * memberships to 'n' classes. This image is conveniently generated by the
 * BayesianClassifierInitializationImageFilter. You may use that filter to
 * generate the membership images or specify your own.
 *
 * \par
 * The output of the filter is a label map (an image of unsigned char's is the
 * default.) with pixel values indicating the classes they correspond to. Pixels
 * with intensity 0 belong to the 0th class, 1 belong to the 1st class etc....
 * The classification is done by applying a Maximum decision rule to the posterior
 * image.
 *
 * \par Parameters
 * The filter optionally allows you to specify a prior image as well. The prior
 * image, if specified must be a VectorImage with as many components as the
 * number of classes. The posterior image is then generated by multiplying the
 * prior image with the membership image. If the prior image is not specified,
 * the posterior image is the same as the membership image. Another way to
 * look at it is that the priors default to having a uniform distribution over
 * the number of classes.
 * Posterior membership of a pixel = Prior * Membership
 *
 * \par
 * The filter optionally accepts a smoothing filter and number of iterations
 * associated with the smoothing filter.
 * The philosophy is that the filter allows you to iteratively
 * smooth the posteriors prior to applying the decision rule. It is hoped
 * that this would yield a better classification. The user will need to plug
 * in his own smoothing filter with all the parameters set.
 *
 * \par Template parameters
 * InputVectorImage, datatype of the output labelmap, precision of the posterior
 * image, precision of the prior image.
 *
 * \author John Melonakos, Georgia Tech
 *
 * \note
 * This work is part of the National Alliance for Medical Image Computing
 * (NAMIC), funded by the National Institutes of Health through the NIH Roadmap
 * for Medical Research, Grant U54 EB005149.
 *
 * \sa VectorImage
 * \sa BayesianClassifierInitializationImageFilter
 * \ingroup ClassificationFilters
 * \ingroup ITKClassifiers
 */
template <typename TInputVectorImage,
          typename TLabelsType = unsigned char,
          typename TPosteriorsPrecisionType = double,
          typename TPriorsPrecisionType = double>
class ITK_TEMPLATE_EXPORT BayesianClassifierImageFilter
  : public ImageToImageFilter<TInputVectorImage, Image<TLabelsType, TInputVectorImage::ImageDimension>>
{
public:
  ITK_DISALLOW_COPY_AND_MOVE(BayesianClassifierImageFilter);

  /** Standard class type aliases. */
  using Self = BayesianClassifierImageFilter;
  using Superclass = ImageToImageFilter<TInputVectorImage, Image<TLabelsType, TInputVectorImage::ImageDimension>>;

  using Pointer = SmartPointer<Self>;
  using ConstPointer = SmartPointer<const Self>;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(BayesianClassifierImageFilter, ImageToImageFilter);

  /** Input and Output image types. */
  using typename Superclass::InputImageType;

  /** Dimension of the input image. */
  static constexpr unsigned int Dimension = InputImageType::ImageDimension;

  using OutputImageType = Image<TLabelsType, Self::Dimension>;
  using InputImagePointer = typename InputImageType::ConstPointer;
  using OutputImagePointer = typename OutputImageType::Pointer;
  using ImageRegionType = typename InputImageType::RegionType;

  /** Input and Output image iterators. */
  using InputImageIteratorType = ImageRegionConstIterator<InputImageType>;
  using OutputImageIteratorType = ImageRegionIterator<OutputImageType>;

  /** Pixel types. */
  using InputPixelType = typename InputImageType::PixelType;
  using OutputPixelType = typename OutputImageType::PixelType;

  /** Image Type and Pixel type for the images representing the Prior
   * probability of a pixel belonging to  a particular class. This image has
   * arrays as pixels, the number of elements in the array is the same as the
   * number of classes to be used. */
  using PriorsImageType = VectorImage<TPriorsPrecisionType, Self::Dimension>;
  using PriorsPixelType = typename PriorsImageType::PixelType;
  using PriorsImagePointer = typename PriorsImageType::Pointer;
  using PriorsImageIteratorType = ImageRegionConstIterator<PriorsImageType>;

  /** Image Type and Pixel type for the images representing the membership of a
   *  pixel to a particular class. This image has arrays as pixels, the number of
   *  elements in the array is the same as the number of classes to be used. */
  using MembershipImageType = TInputVectorImage;
  using MembershipPixelType = typename MembershipImageType::PixelType;
  using MembershipImagePointer = typename MembershipImageType::Pointer;
  using MembershipImageIteratorType = ImageRegionConstIterator<MembershipImageType>;

  /** Image Type and Pixel type for the images representing the Posterior
   * probability of a pixel belonging to a particular class. This image has
   * arrays as pixels, the number of elements in the array is the same as the
   * number of classes to be used. */
  using PosteriorsImageType = VectorImage<TPosteriorsPrecisionType, Self::Dimension>;
  using PosteriorsPixelType = typename PosteriorsImageType::PixelType;
  using PosteriorsImagePointer = typename PosteriorsImageType::Pointer;
  using PosteriorsImageIteratorType = ImageRegionIterator<PosteriorsImageType>;

  /** Decision rule to use for defining the label. */
  using DecisionRuleType = Statistics::MaximumDecisionRule;
  using DecisionRulePointer = DecisionRuleType::Pointer;

  using typename Superclass::DataObjectPointer;

  /** An image from a single component of the Posterior. */
  using ExtractedComponentImageType = itk::Image<TPosteriorsPrecisionType, Self::Dimension>;

  /** Optional Smoothing filter that will be applied to the Posteriors. */
  using SmoothingFilterType = ImageToImageFilter<ExtractedComponentImageType, ExtractedComponentImageType>;

  using SmoothingFilterPointer = typename SmoothingFilterType::Pointer;

  /** Set/Get the smoothing filter that may optionally be applied to the
   *  posterior image. */
  void
  SetSmoothingFilter(SmoothingFilterType *);
  itkGetConstMacro(SmoothingFilter, SmoothingFilterPointer);

  /** Set the priors image. */
  virtual void
  SetPriors(const PriorsImageType *);

  /** Number of iterations to apply the smoothing filter. */
  itkSetMacro(NumberOfSmoothingIterations, unsigned int);
  itkGetConstMacro(NumberOfSmoothingIterations, unsigned int);

  /** This is overloaded to create the Posteriors output image. */
  using DataObjectPointerArraySizeType = ProcessObject::DataObjectPointerArraySizeType;
  using Superclass::MakeOutput;
  DataObjectPointer
  MakeOutput(DataObjectPointerArraySizeType idx) override;

#ifdef ITK_USE_CONCEPT_CHECKING
  // Begin concept checking
  itkConceptMacro(UnsignedIntConvertibleToLabelsCheck, (Concept::Convertible<unsigned int, TLabelsType>));
  itkConceptMacro(PosteriorsAdditiveOperatorsCheck, (Concept::AdditiveOperators<TPosteriorsPrecisionType>));
  itkConceptMacro(IntConvertibleToPosteriorsCheck, (Concept::Convertible<int, TPosteriorsPrecisionType>));
  itkConceptMacro(InputHasNumericTraitsCheck, (Concept::HasNumericTraits<typename InputPixelType::ValueType>));
  itkConceptMacro(PosteriorsHasNumericTraitsCheck, (Concept::HasNumericTraits<TPosteriorsPrecisionType>));
  itkConceptMacro(PriorsHasNumericTraitsCheck, (Concept::HasNumericTraits<TPriorsPrecisionType>));
  itkConceptMacro(
    InputPriorsPosteriorsMultiplyOperatorCheck,
    (Concept::MultiplyOperator<typename InputPixelType::ValueType, PriorsPixelType, PosteriorsPixelType>));
  // End concept checking
#endif

protected:
  BayesianClassifierImageFilter();
  ~BayesianClassifierImageFilter() override = default;
  void
  PrintSelf(std::ostream & os, Indent indent) const override;

  void
  GenerateData() override;

  void
  GenerateOutputInformation() override;

  /** Compute the posteriors using the Bayes rule. If no priors are available,
   *  then the posteriors are just a copy of the memberships.
   *  Computes the labeled map for all combinations of conditions. */
  virtual void
  ComputeBayesRule();

  /** Normalize the posteriors and smooth them using a user-provided. */
  virtual void
  NormalizeAndSmoothPosteriors();

  /** Compute the labeled map based on the Maximum rule applied to the posteriors. */
  virtual void
  ClassifyBasedOnPosteriors();

  /** Get the Posteriors Image. */
  PosteriorsImageType *
  GetPosteriorImage();

private:
  bool m_UserProvidedPriors{ false };

  bool m_UserProvidedSmoothingFilter{ false };

  SmoothingFilterPointer m_SmoothingFilter;

  unsigned int m_NumberOfSmoothingIterations{ 0 };
};
} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#  include "itkBayesianClassifierImageFilter.hxx"
#endif

#endif
