<%
/**
 * Copyright (c) 2000-2005 Liferay, LLC. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
%>

<%@ include file="/html/portal/init.jsp" %>

<%
Group group = (Group)request.getAttribute(WebKeys.GROUP);

Layout selLayout = (Layout)request.getAttribute(WebKeys.SEL_LAYOUT);

String tab = ParamUtil.getString(request, "tab", "layout");
String selLayoutId = null;

if (selLayout == null) {
	tab = "children";
	selLayoutId = Layout.DEFAULT_PARENT_LAYOUT_ID;
}
else {
	if (!PortalUtil.isLayoutParentable(selLayout)) {
		tab = "layout";
	}

	selLayoutId = selLayout.getLayoutId();
}

String path = themeDisplay.getPathMain() + "/portal/personalize?";
if (group != null) {
	PortletURL editGroupPagesURL = new PortletURLImpl(request, PortletKeys.ADMIN, layoutId, true);

	editGroupPagesURL.setWindowState(WindowState.MAXIMIZED);
	editGroupPagesURL.setPortletMode(PortletMode.VIEW);

	editGroupPagesURL.setParameter("struts_action", "/admin/edit_group_pages");

	path = editGroupPagesURL.toString() + "&group_id=" + group.getGroupId() + "&";
}
%>