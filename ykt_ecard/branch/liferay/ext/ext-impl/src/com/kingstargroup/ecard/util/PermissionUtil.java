/**
 * 
 */
package com.kingstargroup.ecard.util;

import javax.servlet.http.HttpServletRequest;

import com.liferay.portal.kernel.util.GetterUtil;
import com.liferay.portal.kernel.util.StringPool;
import com.liferay.portal.model.Company;
import com.liferay.portal.service.UserLocalServiceUtil;
import com.liferay.portal.util.PortalUtil;
import com.liferay.portal.util.WebKeys;

/**
 * Copyright (C), 2000-2005, Kingstar Co., Ltd.<br>
 * File name: PermissionUtil.java<br>
 * Description: <br>
 * Modify History: <br>
 * 操作类型   操作人   操作时间     操作内容<br>
 * ===================================<br>
 *  创建    Xiao Qi  2005-10-13  <br>
 * @author Xiao Qi
 * @version 
 * @since 1.0
 */
public class PermissionUtil {
	public static boolean hasDelayProcess(HttpServletRequest req) {
		
		return true;
	}
	
	public static boolean hasNewsAdmin(HttpServletRequest req) {
		return process(req, NEWS_ADMIN);
	}
	
	public static boolean hasLostFoundAdmin(HttpServletRequest req) {
		return process(req, LOSTFOUND_ADMIN);
	}
	
	public static boolean hasRuleAdmin(HttpServletRequest req) {
		return process(req, RULE_ADMIN);
	}
	
	public static boolean hasMapAdmin(HttpServletRequest req) {
		return process(req, MAP_ADMIN);
	}
	
	public static boolean hasTutorialAdmin(HttpServletRequest req) {
		return process(req, TUTORIAL_ADMIN);
	}
	
	public static boolean hasFAQAdmin(HttpServletRequest req) {
		return process(req, FAQ_ADMIN);
	}
	
	public static boolean hasMsgboardAdmin(HttpServletRequest req) {
		return process(req, MSGBOARD_ADMIN);
	}
	
	public static boolean hasCardTransactionAdmin(HttpServletRequest req) {
		return process(req, CARDTRANSACTION_ADMIN);
	}
	
	public static boolean hasCardlistAdmin(HttpServletRequest req) {
		return process(req, CARDLIST_ADMIN);
	}
	
	public static boolean hasDocumentAdmin(HttpServletRequest req) {
		return process(req, DOCUMENT_ADMIN);
	}
	
	public static boolean hasAdmin(HttpServletRequest req) {
		return process(req, ADMINISTRATOR);
	}
	
	public static boolean hasAboutAdmin(HttpServletRequest req) {
		return process(req, ABOUT_ADMIN);
	}
	
	public static boolean hasHitinfoAdmin(HttpServletRequest req) {
		return process(req, HITINFO_ADMIN);
	}
	
	public static boolean hasServiceinfoAdmin(HttpServletRequest req) {
		return process(req, SERVICEINFO_ADMIN);
	}
	
	public static boolean hasCardProcessAdmin(HttpServletRequest req) {
		return process(req, CARD_PROCESS);
	}
	
	/**
	 * Description:判断当前登录用户是否具有传入的角色权限
	 * @param req
	 * @param roleName
	 * @return
	 * Modify History:
	 */
	static boolean process(HttpServletRequest req, String roleName) {
		long userId = getUserId(req);
		
		try {
			Company company = PortalUtil.getCompany(req);
			return com.liferay.portal.service.RoleServiceUtil.hasUserRole(userId, company.getCompanyId(), roleName, false);
		} catch (Exception ex) {
			ex.printStackTrace();
			return false;
		}
	}
	static long getUserId(HttpServletRequest req) {
		if(req.getAttribute(WebKeys.USER_ID)==null){
			return 0;			
		}else{
			return (Long)req.getAttribute(WebKeys.USER_ID);  
		}
	}
	
	public final static String ADMINISTRATOR = "Administrator";
	public final static String NEWS_ADMIN = "Administrator";
	public final static String LOSTFOUND_ADMIN = "Administrator";
	public final static String RULE_ADMIN = "Administrator";
	public final static String MAP_ADMIN = "Administrator";
	public final static String TUTORIAL_ADMIN = "Administrator";
	public final static String FAQ_ADMIN = "Administrator";
	public final static String MSGBOARD_ADMIN = "Administrator";
	public final static String CARDTRANSACTION_ADMIN ="Administrator";
	public final static String CARDLIST_ADMIN = "Administrator";
	public final static String DOCUMENT_ADMIN = "Administrator";
	public final static String ABOUT_ADMIN = "Administrator";
	public final static String HITINFO_ADMIN = "Administrator";
	public final static String SERVICEINFO_ADMIN = "Administrator";
	public final static String CARD_PROCESS = "Administrator";

}
