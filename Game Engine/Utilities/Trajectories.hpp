#pragma once

#include "vector"

#include "Math.h"

enum TrajectoryType
{
	TrajectoryType_DEFAULT,

	Linear,
	LineStrip,
	LineLoop,
	Circle
};

enum TrajectoryDirectionType
{
	TrajectoryDirectionType_DEFAULT,

	normal,
	alternate
};

class Trajectory
{
public:
	/** Constructor */
	Trajectory(int count)
		: iterationCount(count)
	{  }

	/** Tick function */
	virtual void applyTrajectory(Vector3& currentPosition, Vector3& currentRotation, float deltaTime) = 0;

protected:
	int iterationCount;
};

class LinearTrajectory : public Trajectory
{
public:
	/** Constructor */
	LinearTrajectory(Vector3 start, Vector3 end, float desiredSpeed, int count, TrajectoryDirectionType dirType)
		: Trajectory(count), startPoint(start), endPoint(end), speed(desiredSpeed), direction(dirType)
	{  }


	/**
		V1 : Interpolate float from Current to Target. Scaled by distance to Target, so it has a strong start speed and ease out
		V2 : Linearly interpolates between A and B based on Alpha (100% of A when Alpha=0 and 100% of B when Alpha=1)

		Deocamdata folosesc V1
	*/

	void applyTrajectory(Vector3& currentPosition, Vector3& currentRotation, float deltaTime) override
	{
		if (speed <= 0.0f || iterationCount == 0)
		{
			return;
		}

		// distance to reach
		Vector3 dist = endPoint - currentPosition;

		// If distance is too small, just set the desired location
		if (dist.Length() <= 2.0f)
		{
			currentPosition = endPoint;

			switch (direction)
			{
				case TrajectoryDirectionType::normal:
				{
					currentPosition = startPoint;

					if (iterationCount > 0)
					{
						iterationCount--;
					}
				}
				break;

				case TrajectoryDirectionType::alternate:
				{
					static int iteration = 1;
					iteration = 1 - iteration;
					if (iteration == 1)
					{
						if (iterationCount > 0)
						{
							iterationCount--;
						}
					}

					// TODO : rotate object
					currentRotation.y += getRadians(180.0f);

					std::swap(startPoint, endPoint);
				}
				break;
			}
		}

		Vector3 deltaMove = dist * clamp(deltaTime * speed, 0.0f, 1.0f);
		currentPosition += deltaMove;
	}

private:
	Vector3 startPoint;
	Vector3 endPoint;

	float speed;
	TrajectoryDirectionType direction;
};

class LineStripTrajectory : public Trajectory
{
public:
	/** Constructor */
	LineStripTrajectory(std::vector<Vector3> points, float desiredSpeed, int count, TrajectoryDirectionType dirType)
		: Trajectory(count), points(points), speed(desiredSpeed), direction(dirType)
	{	
		nextPointIndex = 1;
		nextIndex = 1;
	}

	void applyTrajectory(Vector3& currentPosition, Vector3& currentRotation, float deltaTime) override
	{
		if (speed <= 0.0f || iterationCount == 0)
		{
			return;
		}

		Vector3* nextPoint = &points[nextPointIndex];

		// distance to reach
		Vector3 dist = *nextPoint - currentPosition;

		// If distance is too small, just set the desired location
		if (dist.Length() <= 2.0f)
		{
			currentPosition = *nextPoint;
			nextPointIndex += nextIndex;

			if (nextPointIndex == points.size() || nextPointIndex == -1)
			{
				switch (direction)
				{
					case TrajectoryDirectionType::normal:
					{
						if (iterationCount > 0)
						{
							iterationCount--;
						}
						nextPointIndex = 0;
						currentPosition = points[nextPointIndex];
					}
					break;
					
					case TrajectoryDirectionType::alternate:
					{
						if (nextIndex == 1)
						{
							nextIndex = -1;
							nextPointIndex = points.size() - 1;
						}
						else if (nextIndex == -1)
						{
							nextIndex = 1;
							nextPointIndex = 0;

							if (iterationCount > 0)
							{
								iterationCount--;
							}
						}
					}
					break;
				}
			}
		}

		Vector3 deltaMove = dist * clamp(deltaTime * speed, 0.0f, 1.0f);
		currentPosition += deltaMove;
	}

private:
	std::vector<Vector3> points;

	float speed;
	TrajectoryDirectionType direction;

	int nextPointIndex;
	int nextIndex;
};

class LineLoopTrajectory : public Trajectory
{
public:
	/** Constructor */
	LineLoopTrajectory(std::vector<Vector3> points, float desiredSpeed, int count)
		: Trajectory(count), points(points), speed(desiredSpeed)
	{
		nextPointIndex = 1;
	}

	void applyTrajectory(Vector3& currentPosition, Vector3& currentRotation, float deltaTime) override
	{
		if (speed <= 0.0f || iterationCount == 0)
		{
			return;
		}

		Vector3* nextPoint = &points[nextPointIndex];

		// distance to reach
		Vector3 dist = *nextPoint - currentPosition;

		// If distance is too small, just set the desired location
		if (dist.Length() <= 2.0f)
		{
			currentPosition = *nextPoint;
			nextPointIndex += 1;

			if (nextPointIndex == points.size())
			{
				nextPointIndex = 0;
			}
			
			if (nextPointIndex == 1)
			{
				if (iterationCount > 0)
				{
					iterationCount--;
				}
			}
		}

		Vector3 deltaMove = dist * clamp(deltaTime * speed, 0.0f, 1.0f);
		currentPosition += deltaMove;
	}

private:
	std::vector<Vector3> points;

	float speed;
	int nextPointIndex;
};

class CircleTrajectory : public Trajectory
{
public:
	/** Constructor */
	CircleTrajectory(Vector3 center, Vector3 rotationPlane, float radius, float desiredSpeed, int count)
		: Trajectory(count), center(center), rotationPlane(rotationPlane), radius(radius), speed(desiredSpeed)
	{
		
	}

	void applyTrajectory(Vector3& currentPosition, Vector3& currentRotation, float deltaTime) override
	{
		// TODO

	}

private:
	Vector3 center;
	Vector3 rotationPlane;
	float radius;
	float speed;
};

