// Extracted from: ModelView.c++ - an Abstract Base Class for a combined Model and View for OpenGL

#include "ModelView.h"

void ModelView::addToGlobalPan(double dxInLDS, double dyInLDS, double dzInLDS)
{
    using namespace cryph;

    double dxInEC = (0.5 * dxInLDS)*(last_ecXmax-last_ecXmin);
    double dyInEC = (0.5 * dyInLDS)*(last_ecYmax-last_ecYmin);

    AffVector vector(dxInEC, dyInEC, dzInLDS);

    dynamic_view = Matrix4x4::translation(vector) * dynamic_view;

}

void ModelView::addToGlobalRotationDegrees(double rx, double ry, double rz)
{
    using namespace cryph;
    dynamic_view = Matrix4x4::xRotationDegrees(rx) * Matrix4x4::yRotationDegrees(ry) * Matrix4x4::zRotationDegrees(rz) * dynamic_view;
}

void ModelView::getMatrices(cryph::Matrix4x4& mc_ec, cryph::Matrix4x4& ec_lds)
{
    using namespace cryph;

    cryph::Matrix4x4 M_ECu = cryph::Matrix4x4::lookAt(ModelView::eye,
    ModelView::center, ModelView::up);

    const AffVector trans_vec(0,0,ModelView::distEyeCenter);
    const Matrix4x4 pre = Matrix4x4::translation(trans_vec);
    const Matrix4x4 post = Matrix4x4::translation(-trans_vec);

    //    For project 3:
    mc_ec = post * dynamic_view * pre * M_ECu;


    const float delta_mcX = mcRegionOfInterest[1] - mcRegionOfInterest[0];
    const float delta_mcY = mcRegionOfInterest[3] - mcRegionOfInterest[2];
    const float delta_mcZ = mcRegionOfInterest[5] - mcRegionOfInterest[4];

    double maxDelta = delta_mcX;
    if( delta_mcY > maxDelta )
    maxDelta = delta_mcY;
    if( delta_mcZ > maxDelta )
    maxDelta = delta_mcZ;

    double halfWidth = 0.5 * maxDelta * dynamic_zoomScale;
    //    2.b. In project 3 & 4: Scale "halfWidth" by "dynamic_zoomScale"
    //    2.c. initialize the XY direction of the view volume as:
    last_ecXmin = -halfWidth;
    last_ecXmax = halfWidth; // instance variables; see...

    last_ecYmin = -halfWidth;
    last_ecYmax = halfWidth; // ... ModelView.h


    //    2.d. Use ModelView::matchAspectRatio to alter one of these pairs.
    matchAspectRatio( last_ecXmin, last_ecXmax,
                  last_ecYmin, last_ecYmax,
      Controller::getCurrentController()->getViewportAspectRatio()
        );

    if (ModelView::projType == ORTHOGONAL)
    ec_lds = cryph::Matrix4x4::orthogonal(last_ecXmin, last_ecXmax, last_ecYmin, last_ecYmax,
    ModelView::ecZmin, ModelView::ecZmax);
    else if (ModelView::projType == PERSPECTIVE)
    ec_lds = cryph::Matrix4x4::perspective(ModelView::ecZpp,
    last_ecXmin, last_ecXmax, last_ecYmin, last_ecYmax, ModelView::ecZmin, ModelView::ecZmax);
    else // Must be OBLIQUE
    ec_lds = cryph::Matrix4x4::oblique(ModelView::ecZpp,
    last_ecXmin, last_ecXmax, last_ecYmin, last_ecYmax, ModelView::ecZmin, ModelView::ecZmax,
    ModelView::obliqueProjectionDir);



    //std::cout << ec_lds;
    //std::cout << "\n";

    // THEN IN THE CALLER OF THIS METHOD:
    //
    // float mat[16];
    // glUniformMatrix4fv(shaderIF->ppuLoc("mc_ec"), 1, false, mc_ec.extractColMajor(mat));
    // glUniformMatrix4fv(shaderIF->ppuLoc("ec_lds"), 1, false, ec_lds.extractColMajor(mat));
    //
    // (The extractColMajor method copies the elements of the matrix into the given
    // array which is assumed to be of length 16. It then returns the array pointer
    // so it can be used as indicated in the two calls. Since the array is immediately
    // copied by glUniformMatrix to the GPU, "mat" can be reused as indicated.)
}

void ModelView::scaleGlobalZoom(double multiplier)
{
    dynamic_zoomScale *= 1/multiplier;
}
