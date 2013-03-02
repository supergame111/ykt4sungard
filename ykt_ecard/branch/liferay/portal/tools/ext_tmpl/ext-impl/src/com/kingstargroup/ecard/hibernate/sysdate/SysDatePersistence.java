/**
 * 
 */
package com.kingstargroup.ecard.hibernate.sysdate;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.hibernate.Hibernate;
import org.hibernate.HibernateException;
import org.hibernate.SQLQuery;
import org.hibernate.Session;

import com.kingstargroup.ecard.KSConfiguration;
import com.kingstargroup.ecard.exceptions.DBSystemException;
import com.liferay.portal.kernel.util.StringMaker;
import com.liferay.portal.service.persistence.BasePersistence;

/**
 * Copyright (C), 2000-2005, Kingstar Co., Ltd.<br>
 * File name: SysDatePersistence.java<br>
 * Description: <br>
 * Modify History: <br>
 * �������� ������ ����ʱ�� ��������<br>
 * ===================================<br>
 * ���� Xiao Qi 2005-9-29 <br>
 * 
 * @author Xiao Qi
 * @version
 * @since 1.0
 */
public class SysDatePersistence extends BasePersistence {

	/**
	 * Description:
	 * 
	 * @param days
	 *            ��Ҫ���������
	 * @return
	 * @throws DBSystemException
	 *             Modify History:
	 */
	public String getSysDate(int days) throws DBSystemException {
		Session s = null;
		try {
			s = openSession();
			String validDays = null;
			if ("db2".equals(KSConfiguration.getInstance().getProperty(
					"constant.db.config", "db2"))) {
				if (String.valueOf(days).indexOf('-') == -1) {
					validDays = "+" + days;
				}
				StringMaker buff = new StringMaker();
				buff
						.append(
								"select substr(d,1,4)||substr(d,6,2)||substr(d,9,2)")
						.append(
								"||substr(t,1,2)||substr(t,4,2)||substr(t,7,2) tdate  ")
						.append("  from (SELECT char(CURRENT date").append(
								validDays).append(
								" DAYS,ISO) d ,char(current time,ISO) t ")
						.append(" FROM SYSIBM.SYSDUMMY1 ) aaa");
				SQLQuery query = s.createSQLQuery(buff.toString());
				query.addScalar("tdate", Hibernate.STRING);
				return (String) query.uniqueResult();
			} else if ("oracle".equals(KSConfiguration.getInstance()
					.getProperty("constant.db.config", "db2"))) {
				StringBuffer buff = new StringBuffer();
				buff.append("select to_char(sysdate ").append("+ interval '")
						.append(days).append(
								"' day,'yyyymmddhh24miss') tdate from dual ");
				SQLQuery query = s.createSQLQuery(buff.toString());
				query.addScalar("tdate", Hibernate.STRING);
				String returnString = (String) query.uniqueResult();
				return returnString;
			} else {
				return "";
			}

		} catch (HibernateException ex) {
			_log.error(ex);
			throw new DBSystemException(ex.getMessage());
		} finally {
			closeSession(s);
		}
	}

	private static final Log _log = LogFactory.getLog(SysDatePersistence.class);

}