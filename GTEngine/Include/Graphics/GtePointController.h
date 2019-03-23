// David Eberly, Geometric Tools, Redmond WA 98052
// Copyright (c) 1998-2019
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
// File Version: 3.22.0 (2019/02/03)

#pragma once

#include <Mathematics/GteVector3.h>
#include <Graphics/GteController.h>
#include <Graphics/GteVertexBuffer.h>

namespace gte
{
    class GTE_IMPEXP PointController : public Controller
    {
    public:
        // Abstract base class. The object to which this is attached must be
        // a Visual that stores a points geometric primitive.  The index
        // buffer must be of type IP_POLYPOINT.
        typedef std::function<void(std::shared_ptr<VertexBuffer> const&)> Updater;

        virtual ~PointController();

    protected:
        PointController(Updater const& postUpdate);

    public:
        // The system motion, in local coordinates.  The velocity vectors
        // must be unit length.
        float systemLinearSpeed;
        float systemAngularSpeed;
        Vector3<float> systemLinearAxis;
        Vector3<float> systemAngularAxis;

        // Point motion, in the model space of the system.  The velocity
        // vectors must be unit length.  In applications where the points
        // represent a rigid body, you might choose the origin of the system
        // to be the center of mass of the points and the coordinate axes to
        // correspond to the principal directions of the inertia tensor.
        inline size_t GetNumPoints() const
        {
            return mPointLinearSpeed.size();
        }

        inline std::vector<float> const& GetPointLinearSpeed() const
        {
            return mPointLinearSpeed;
        }

        inline std::vector<float> const& GetPointAngularSpeed() const
        {
            return mPointAngularSpeed;
        }

        inline std::vector<Vector3<float>> const& GetPointLinearAxis() const
        {
            return mPointLinearAxis;
        }

        inline std::vector<Vector3<float>> const& GetPointAngularAxis() const
        {
            return mPointAngularAxis;
        }

        // The animation update.  The application time is in milliseconds.
        virtual bool Update(double applicationTime);

    protected:
        // Override the base-class member function in order to verify that
        // the object is Visual with a vertex format and vertex buffer that
        // satisfy the preconditions of the PointController.
        virtual void SetObject(ControlledObject* object) override;

        // This class computes the new positions and orientations from the
        // motion parameters.  Derived classes should update the motion
        // parameters and then either call the base class update methods or
        // provide its own update methods for position and orientation.
        virtual void UpdateSystemMotion(float ctrlTime);
        virtual void UpdatePointMotion(float ctrlTime);

        std::vector<float> mPointLinearSpeed;
        std::vector<float> mPointAngularSpeed;
        std::vector<Vector3<float>> mPointLinearAxis;
        std::vector<Vector3<float>> mPointAngularAxis;

        Updater mPostUpdate;
    };
}
