package org.king.check.domain;
// Generated by MyEclipse - Hibernate Tools

import java.util.Date;


/**
 * Leaveinfo generated by MyEclipse - Hibernate Tools
 */
public class Leaveinfo extends AbstractLeaveinfo implements java.io.Serializable {

    // Constructors

    /** default constructor */
    public Leaveinfo() {
    }

    
    /** full constructor */
    public Leaveinfo(String description, Date startdate, Date enddate,Department 

department,Clerk clerk,Date recorddate,Integer operatorid,String flag) {
        super(description, startdate, enddate,department,clerk,recorddate,operatorid,flag);       

 
    }


	public String toString() {
		// TODO Auto-generated method stub
		return null;
	}


	public boolean equals(Object arg0) {
		// TODO Auto-generated method stub
		return false;
	}


	public int hashCode() {
		// TODO Auto-generated method stub
		return 0;
	}
   
}