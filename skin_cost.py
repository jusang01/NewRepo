# -*- coding: utf-8 -*-
#大客壁板算例
import math

# char = [chr(code) for code in range(945,970)]
# codelist = [code for code in range(945,970)]
# print(char)
def t_skin_yunsuputie(step,G,η,ν):
    #功能：蒙皮铺贴成本模型，基于重量不考虑铺层角度的匀速铺贴模型.
    #t：工时，min.
    #step:准备时间，min.
    #G:净重.
    #η:材料利用率.
    #ν:铺贴速率，kg/min.
    t_zhongliang = step + G / (η * ν)
    return t_zhongliang
# a = t_skin_yunsuputie(30,14.76190476,0.21,0.010042112)
# print(a)
def tman_skin_guangyan(setup,delay,χ,ν,τ=0,Noper=1):
    #功能：蒙皮广延型工序成本模型，计算工装准备、下料、定位、制袋、脱模、机加、检测等.
    #tman：工时，min.
    #setup:准备时间，min.
    #delay:延迟时间,min.
    #χ:特征尺寸，kg、mm、m2.
    #ν:速率，kg/min、mm/min、m2/min.
    #τ:根据目前研制进展，τ取0.
    #Noper:不知道是啥，暂记为1.

    tman_guanyan = setup + (delay + math.sqrt(math.pow(χ/ν,2)+ 2 * τ* χ/ν)) * Noper
    return tman_guanyan
# a = tman_CH_guangyan(30,0,14.76190476,0.091461616)
# print(a)
def Part_Labor_Cost_skin(Man_Hours,Labor_Rate,Machine_Hours,Machine_Rate):
    #功能：计算蒙皮各工序下的工时费.
    #Man_Hours：人工工时，min.
    #Labor_Rate:人工费率，元/h.
    #Machine_Hours:设备加工时间,min.
    #Machine_Rate:设备费率，元/h.
    Part_Labor_Cost = Man_Hours * Labor_Rate / 60 + Machine_Hours * Machine_Rate / 60
    return Part_Labor_Cost
#Cost per unit = Material cost per unit + Labor cost per unit
def Single_Part_Cost_skin(Material_cost_per_unit,Labor_cost_per_unit):
    #功能：计算蒙皮单件成本.
    #Material_cost_per_unit：单件材料费，元.
    #Labor_cost_per_unit:单件工时费，元.
    Single_Part_Cost = Material_cost_per_unit + Labor_cost_per_unit
    return Single_Part_Cost




