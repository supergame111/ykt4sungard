package org.king.check.domain;



/**
 * AbstractTcheckattreport generated by MyEclipse - Hibernate Tools
 */

public abstract class AbstractTcheckattreport extends org.king.framework.domain.BaseObject implements java.io.Serializable {


    // Fields    

     private TcheckattreportId id;
     private String deptId;
     private String ifoffice;
     private Integer worktimeValue;
     private Integer factValue;
     private Integer handTimes;
     private Integer overtimeValue;
     private Integer tworestValue;
     private Integer holidayValue;
     private Integer delayValue;
     private Integer leaveEarly;
     private Integer leaveValue;
     private Integer evecion;
     private Integer sickLeave;
     private Integer affairLeave;
     private Integer mateLeave;
     private Integer weddingLeave;
     private Integer holleaveValue;
     private Integer hurtLeave;
     private Integer visitLeave;
     private Integer funeralLeave;
     private Integer suckleLeave;
     private Integer otherLeave;
     private Integer ext1;
     private Integer ext2;
     private Integer ext3;
     private Integer ext4;
     private Integer ext5;


    // Constructors

    /** default constructor */
    public AbstractTcheckattreport() {
    }

	/** minimal constructor */
    public AbstractTcheckattreport(TcheckattreportId id) {
        this.id = id;
    }
    
    /** full constructor */
    public AbstractTcheckattreport(TcheckattreportId id, String deptId, String ifoffice, Integer worktimeValue, Integer factValue, Integer handTimes, Integer overtimeValue, Integer tworestValue, Integer holidayValue, Integer delayValue, Integer leaveEarly, Integer leaveValue, Integer evecion, Integer sickLeave, Integer affairLeave, Integer mateLeave, Integer weddingLeave, Integer holleaveValue, Integer hurtLeave, Integer visitLeave, Integer funeralLeave, Integer suckleLeave, Integer otherLeave, Integer ext1, Integer ext2, Integer ext3, Integer ext4, Integer ext5) {
        this.id = id;
        this.deptId = deptId;
        this.ifoffice = ifoffice;
        this.worktimeValue = worktimeValue;
        this.factValue = factValue;
        this.handTimes = handTimes;
        this.overtimeValue = overtimeValue;
        this.tworestValue = tworestValue;
        this.holidayValue = holidayValue;
        this.delayValue = delayValue;
        this.leaveEarly = leaveEarly;
        this.leaveValue = leaveValue;
        this.evecion = evecion;
        this.sickLeave = sickLeave;
        this.affairLeave = affairLeave;
        this.mateLeave = mateLeave;
        this.weddingLeave = weddingLeave;
        this.holleaveValue = holleaveValue;
        this.hurtLeave = hurtLeave;
        this.visitLeave = visitLeave;
        this.funeralLeave = funeralLeave;
        this.suckleLeave = suckleLeave;
        this.otherLeave = otherLeave;
        this.ext1 = ext1;
        this.ext2 = ext2;
        this.ext3 = ext3;
        this.ext4 = ext4;
        this.ext5 = ext5;
    }

   
    // Property accessors

    public TcheckattreportId getId() {
        return this.id;
    }
    
    public void setId(TcheckattreportId id) {
        this.id = id;
    }

    public String getDeptId() {
        return this.deptId;
    }
    
    public void setDeptId(String deptId) {
        this.deptId = deptId;
    }

    public String getIfoffice() {
        return this.ifoffice;
    }
    
    public void setIfoffice(String ifoffice) {
        this.ifoffice = ifoffice;
    }

    public Integer getWorktimeValue() {
        return this.worktimeValue;
    }
    
    public void setWorktimeValue(Integer worktimeValue) {
        this.worktimeValue = worktimeValue;
    }

    public Integer getFactValue() {
        return this.factValue;
    }
    
    public void setFactValue(Integer factValue) {
        this.factValue = factValue;
    }

    public Integer getHandTimes() {
        return this.handTimes;
    }
    
    public void setHandTimes(Integer handTimes) {
        this.handTimes = handTimes;
    }

    public Integer getOvertimeValue() {
        return this.overtimeValue;
    }
    
    public void setOvertimeValue(Integer overtimeValue) {
        this.overtimeValue = overtimeValue;
    }

    public Integer getTworestValue() {
        return this.tworestValue;
    }
    
    public void setTworestValue(Integer tworestValue) {
        this.tworestValue = tworestValue;
    }

    public Integer getHolidayValue() {
        return this.holidayValue;
    }
    
    public void setHolidayValue(Integer holidayValue) {
        this.holidayValue = holidayValue;
    }

    public Integer getDelayValue() {
        return this.delayValue;
    }
    
    public void setDelayValue(Integer delayValue) {
        this.delayValue = delayValue;
    }

    public Integer getLeaveEarly() {
        return this.leaveEarly;
    }
    
    public void setLeaveEarly(Integer leaveEarly) {
        this.leaveEarly = leaveEarly;
    }

    public Integer getLeaveValue() {
        return this.leaveValue;
    }
    
    public void setLeaveValue(Integer leaveValue) {
        this.leaveValue = leaveValue;
    }

    public Integer getEvecion() {
        return this.evecion;
    }
    
    public void setEvecion(Integer evecion) {
        this.evecion = evecion;
    }

    public Integer getSickLeave() {
        return this.sickLeave;
    }
    
    public void setSickLeave(Integer sickLeave) {
        this.sickLeave = sickLeave;
    }

    public Integer getAffairLeave() {
        return this.affairLeave;
    }
    
    public void setAffairLeave(Integer affairLeave) {
        this.affairLeave = affairLeave;
    }

    public Integer getMateLeave() {
        return this.mateLeave;
    }
    
    public void setMateLeave(Integer mateLeave) {
        this.mateLeave = mateLeave;
    }

    public Integer getWeddingLeave() {
        return this.weddingLeave;
    }
    
    public void setWeddingLeave(Integer weddingLeave) {
        this.weddingLeave = weddingLeave;
    }

    public Integer getHolleaveValue() {
        return this.holleaveValue;
    }
    
    public void setHolleaveValue(Integer holleaveValue) {
        this.holleaveValue = holleaveValue;
    }

    public Integer getHurtLeave() {
        return this.hurtLeave;
    }
    
    public void setHurtLeave(Integer hurtLeave) {
        this.hurtLeave = hurtLeave;
    }

    public Integer getVisitLeave() {
        return this.visitLeave;
    }
    
    public void setVisitLeave(Integer visitLeave) {
        this.visitLeave = visitLeave;
    }

    public Integer getFuneralLeave() {
        return this.funeralLeave;
    }
    
    public void setFuneralLeave(Integer funeralLeave) {
        this.funeralLeave = funeralLeave;
    }

    public Integer getSuckleLeave() {
        return this.suckleLeave;
    }
    
    public void setSuckleLeave(Integer suckleLeave) {
        this.suckleLeave = suckleLeave;
    }

    public Integer getOtherLeave() {
        return this.otherLeave;
    }
    
    public void setOtherLeave(Integer otherLeave) {
        this.otherLeave = otherLeave;
    }

    public Integer getExt1() {
        return this.ext1;
    }
    
    public void setExt1(Integer ext1) {
        this.ext1 = ext1;
    }

    public Integer getExt2() {
        return this.ext2;
    }
    
    public void setExt2(Integer ext2) {
        this.ext2 = ext2;
    }

    public Integer getExt3() {
        return this.ext3;
    }
    
    public void setExt3(Integer ext3) {
        this.ext3 = ext3;
    }

    public Integer getExt4() {
        return this.ext4;
    }
    
    public void setExt4(Integer ext4) {
        this.ext4 = ext4;
    }

    public Integer getExt5() {
        return this.ext5;
    }
    
    public void setExt5(Integer ext5) {
        this.ext5 = ext5;
    }
}