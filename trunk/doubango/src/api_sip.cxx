#include "api_sip.h"
#include "api_engine.h"
#include "api_stack.h"

#include <sofia-sip/auth_module.h>
#include <sofia-sip/su_tag.h> // FIXME: remove
#include <sofia-sip/sip_tag.h> // FIXME: remove

PREF_NAMESPACE_START

#define GET_STACK(stack_id, stk) \
	stack* stk = (stack*)engine_stack_find(stack_id); \
	if(!stk) return ERR_STACK_NOT_FOUND; \
	if(!stk->get_initialized()) return ERR_STACK_NOT_INITIALIZED; \
	if(!stk->get_running()) return ERR_STACK_NOT_RUNNING;

#define SAFE_FREE_HOME_PTR(home, ptr) { if(ptr){ su_free(home, ptr); } }
	

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// sip REGISTER
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* send sip register message */
ERR sip_register(int stack_id)
{
	GET_STACK(stack_id, stk);
	return stk->sip_register();
}

/* send sip unregister (register with expires=0) message*/
ERR sip_unregister(int stack_id)
{
	GET_STACK(stack_id, stk);
	return stk->sip_unregister();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// sip MESSAGE
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* send sip register message */
ERR sip_message(int stack_id, const char* dest_address, const char* content_type, const char* content)
{
	GET_STACK(stack_id, stk);
	return stk->sip_message(dest_address, content_type, content);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// authentication
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ERR auth_set(int stack_id, const char* public_id, const char* private_id, const char* password, const char* realm)
{
	ERR error = ERR_SUCCESS;
	if( !ERR_SUCCEED( (error = auth_set_public_id(stack_id, public_id)) ) ) return error;
	if( !ERR_SUCCEED( (error = auth_set_private_id(stack_id, private_id)) ) ) return error;
	if( !ERR_SUCCEED( (error = auth_set_password(stack_id, password)) ) ) return error;
	if( !ERR_SUCCEED( (error = auth_set_realm(stack_id, realm)) ) ) return error;

	return error;
}

ERR auth_set_displayname(int stack_id, const char* displayname)
{
	GET_STACK(stack_id, stk);
	SAFE_FREE_HOME_PTR(stk->get_home(), stk->get_displayname());
	stk->set_displayname( su_strdup(stk->get_home(), displayname) );
	nua_set_params(stk->get_nua(),
					NUTAG_M_DISPLAY(displayname),
                    TAG_NULL());
	return ERR_SUCCESS;
}

ERR auth_set_public_id(int stack_id, const char* public_id)
{
	GET_STACK(stack_id, stk);
	SAFE_FREE_HOME_PTR(stk->get_home(), stk->get_public_id());
	stk->set_public_id( su_strdup(stk->get_home(), public_id) );
	return ERR_SUCCESS;
}

ERR auth_set_private_id(int stack_id, const char* private_id)
{
	GET_STACK(stack_id, stk);
	SAFE_FREE_HOME_PTR(stk->get_home(), stk->get_private_id());
	stk->set_private_id( su_strdup(stk->get_home(), private_id) );
	return ERR_SUCCESS;
}

ERR auth_set_preferred_id(int stack_id, const char* preferred_id)
{
	return ERR_NOT_IMPLEMENTED;
}

ERR auth_set_password(int stack_id, const char* password)
{
	GET_STACK(stack_id, stk);
	SAFE_FREE_HOME_PTR(stk->get_home(), stk->get_password());
	stk->set_password( su_strdup(stk->get_home(), password) );
	return ERR_SUCCESS;
}

ERR auth_set_realm(int stack_id, const char* realm)
{
	GET_STACK(stack_id, stk);
	SAFE_FREE_HOME_PTR(stk->get_home(), stk->get_realm());
	stk->set_realm( su_strdup(stk->get_home(), realm) );
	nua_set_params(stk->get_nua(),
					AUTHTAG_REALM(realm),
                    TAG_NULL());
	return ERR_SUCCESS;
}

ERR auth_set_operator_id(int stack_id, const char* operator_id)
{
	return ERR_NOT_IMPLEMENTED;
}

ERR auth_set_amf(int stack_id, const char* amf)
{
	return ERR_NOT_IMPLEMENTED;
}

ERR auth_set_privacy(int stack_id, const char* privacy)
{
	GET_STACK(stack_id, stk);
	SAFE_FREE_HOME_PTR(stk->get_home(), stk->get_privacy());
	stk->set_privacy( su_strdup(stk->get_home(), privacy) );
	return ERR_SUCCESS;
}

ERR auth_set_security_mechanism(int stack_id, const char* security_mechanism)
{
	return ERR_NOT_IMPLEMENTED;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// network
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ERR network_set(int stack_id, const char* pcscf_ip, int pcscf_port, const char* transport)
{
	return ERR_NOT_IMPLEMENTED;
}

ERR network_set_pcscf(int stack_id, const char* pcscf_ip, int pcscf_port)
{
	GET_STACK(stack_id, stk);
	char* pcscf = su_sprintf(stk->get_home(), "sip:%s:%d", pcscf_ip, pcscf_port);
	nua_set_params(stk->get_nua(),
					NUTAG_PROXY(pcscf),
					NUTAG_OUTBOUND("no-validate"),
                    TAG_NULL());
	su_free(stk->get_home(), pcscf);

	return ERR_SUCCESS;
}

ERR network_set_transport(int stack_id, const char* transport)
{
	return ERR_NOT_IMPLEMENTED;
}

/* SIP Header: P-Access-Network-Info
	RFC 3455
*/
ERR network_set_net_info(int stack_id, const char* net_info)
{
	return ERR_NOT_IMPLEMENTED;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// qos
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ERR qos_set_audio_bandwith(int stack_id, int audio_bandwith)
{
	return ERR_NOT_IMPLEMENTED;
}

ERR qos_set_audio_dscp(int stack_id, int audio_dscp)
{
	return ERR_NOT_IMPLEMENTED;
}

ERR qos_set_video_bandwith(int stack_id, int video_bandwith)
{
	return ERR_NOT_IMPLEMENTED;
}

ERR qos_set_video_dscp(int stack_id, int video_dscp)
{
	return ERR_NOT_IMPLEMENTED;
}

ERR qos_set_msrp_bandwith(int stack_id, int msrp_bandwith)
{
	return ERR_NOT_IMPLEMENTED;
}

ERR qos_set_msrp_dscp(int stack_id, int msrp_dscp)
{
	return ERR_NOT_IMPLEMENTED;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ipsec
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ERR ipsec_set(int stack_id, const char* mode, const char* protocol, const char* auth_algo, const char* enc_algo)
{
	return ERR_NOT_IMPLEMENTED;
}

ERR ipsec_set_mode(int stack_id, const char* mode)
{
	return ERR_NOT_IMPLEMENTED;
}

ERR ipsec_set_protocol(int stack_id, const char* protocol)
{
	return ERR_NOT_IMPLEMENTED;
}

ERR ipsec_set_auth_algo(int stack_id, const char* auth_algo)
{
	return ERR_NOT_IMPLEMENTED;
}

ERR ipsec_set_enc_algo(int stack_id, const char* enc_algo)
{
	return ERR_NOT_IMPLEMENTED;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// tls
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ERR tls_set_priv_key_path(int stack_id, const char* priv_key_path)
{
	return ERR_NOT_IMPLEMENTED;
}

ERR tls_set_pub_key_path(int stack_id, const char* pub_key_path)
{
	return ERR_NOT_IMPLEMENTED;
}

ERR tls_set_cert_path(int stack_id, const char* cert_path)
{
	return ERR_NOT_IMPLEMENTED;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// sigcomp
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ERR sigcomp_set_comp_id(int stack_id, const char* comp_id)
{
	return ERR_NOT_IMPLEMENTED;
}

ERR sigcomp_enable_sipdict(int stack_id, bool enable)
{
	return ERR_NOT_IMPLEMENTED;
}

ERR sigcomp_enable_presdict(int stack_id, bool enable)
{
	return ERR_NOT_IMPLEMENTED;
}

ERR sigcomp_set_dms(int stack_id, int dms)
{
	return ERR_NOT_IMPLEMENTED;
}

ERR sigcomp_set_sms(int stack_id, int sms)
{
	return ERR_NOT_IMPLEMENTED;
}

ERR sigcomp_set_cpb(int stack_id, int cpb)
{
	return ERR_NOT_IMPLEMENTED;
}


PREF_NAMESPACE_END