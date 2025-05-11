#!/usr/bin/env python3
import numpy as np

class Avoider:
    def __init__(self, vel_obj, obstacle_threshold=0.7, normal_lin_vel=0.2,
                 trans_lin_vel=-0.05, trans_ang_vel=1.0):
        self.vel_obj = vel_obj
        self.OBSTACLE_DIST = obstacle_threshold
        self.NORMAL_LIN_VEL = normal_lin_vel
        self.TRANS_LIN_VEL = trans_lin_vel
        self.TRANS_ANG_VEL = trans_ang_vel

        self.REGION_NAMES = [
            "front_C", "front_L", "left_R",
            "left_C", "left_L", "back_R",
            "back_C", "back_L", "right_R",
            "right_C", "right_L", "front_R"
        ]

        self.Regions_Distances = {
                            "front_C":  0, "front_L":  1, "left_R" :  2,
                            "left_C" :  3, "left_L" :  4, "back_R" :  5,
                            "back_C" :  6, "back_L" : -5, "right_R": -4,
                            "right_C": -3, "right_L": -2, "front_R": -1,}

        self.Regions_Report = {region: [] for region in self.REGION_NAMES}

    def identify_regions(self, scan):
        ranges = np.array(scan.ranges)
        ranges[ranges == float('inf')] = 10.0
        ranges = np.nan_to_num(ranges, nan=10.0)

        total_points = len(ranges)
        points_per_region = total_points // 12

        # Assign ranges to 12 regions
        for i, region in enumerate(self.REGION_NAMES):
            start_idx = i * points_per_region
            end_idx = start_idx + points_per_region
            region_values = ranges[start_idx:end_idx]
            # Only consider obstacle distances under threshold
            self.Regions_Report[region] = region_values[region_values <= self.OBSTACLE_DIST]

    def print_obstacle_regions(self):
        print("\n[Obstacle Report]")
        for region in ["front_L", "front_C", "front_R", "left_R", "right_L"]:
            status = "clear" if len(self.Regions_Report[region]) == 0 else f"{len(self.Regions_Report[region])} obstacle(s)"
            print(f"{region}: {status}")


    def _clearance_test(self, goalZone):

        goal = goalZone
        closest = 10e6
        regional_dist = 0
        maxima = {"destination": "back_C", "distance": 10e-6}
        for region in self.Regions_Report.items():
            regional_dist = abs(self.Regions_Distances[region[0]]-self.Regions_Distances[goal])
            #if there're no obstacles in that region
            if not len(region[1]):
                #check if it's the cheapest option
                if (regional_dist < closest):
                    closest = regional_dist
                    maxima["distance"] = self.OBSTACLE_DIST
                    maxima["destination"] = region[0]
            #check if it's the clearest option
            elif(max(region[1]) > maxima["distance"]):
                maxima["distance"] = max(region[1])
                maxima["destination"] = region[0]
        #calculate the cost to the chosen orientation
        regional_dist = self.Regions_Distances[maxima["destination"]]-self.Regions_Distances[goal]
        
        # Return whether to act or not, and the angular velocity with the appropriate sign
        return (closest != 0), (regional_dist/[abs(regional_dist) if regional_dist != 0 else 1][0])*self.TRANS_ANG_VEL

    def avoid(self):
        self.print_obstacle_regions()

        danger_zones = ["front_C", "front_L", "front_R", "left_R", "right_L"]
        blocked_zones = [zone for zone in danger_zones if len(self.Regions_Report[zone]) > 0]

        if not blocked_zones:
            self._set_velocity(self.NORMAL_LIN_VEL, 0.0)
            return self.vel_obj
        
        if "front_C" in blocked_zones:
            if "front_L" in blocked_zones:
                self._set_velocity(self.TRANS_LIN_VEL, -self.TRANS_ANG_VEL)
                return self.vel_obj
            elif "front_R" in blocked_zones:
                self._set_velocity(self.TRANS_LIN_VEL, self.TRANS_ANG_VEL)
                return self.vel_obj
        elif "front_L" in blocked_zones and "front_R" in blocked_zones:
            if "left_R" in blocked_zones:
                self._set_velocity(self.TRANS_LIN_VEL, -self.TRANS_ANG_VEL)
                return self.vel_obj
            elif "right_L" in blocked_zones:
                self._set_velocity(self.TRANS_LIN_VEL, self.TRANS_ANG_VEL)
                return self.vel_obj
            else:
                self._set_velocity(self.TRANS_LIN_VEL, self.TRANS_ANG_VEL)
                return self.vel_obj
        elif "front_L" in blocked_zones:
            self._set_velocity(self.TRANS_LIN_VEL, -self.TRANS_ANG_VEL)
            return self.vel_obj
        elif "front_R" in blocked_zones:
            self._set_velocity(self.TRANS_LIN_VEL, self.TRANS_ANG_VEL)
            return self.vel_obj
        

        #should_rotate, rotation_vel = self._clearance_test("front_C")
        #if should_rotate:
            #self._set_velocity(0.0, rotation_vel)
        #else:
            # fallback: dừng lại
            #self._set_velocity(0.0, 0.0)
        self._set_velocity(0, 0)
        return self.vel_obj




    def _set_velocity(self, lin_x, ang_z):
        self.vel_obj.linear.x = lin_x
        self.vel_obj.linear.y = 0
        self.vel_obj.linear.z = 0
        self.vel_obj.angular.x = 0
        self.vel_obj.angular.y = 0
        self.vel_obj.angular.z = ang_z