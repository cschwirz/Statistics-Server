<?php

/**
 * Give general statistics, i.e. technical details.
 */
function statistics_server_general() {
  $result = db_query('SELECT id, name, url, description FROM {statistics_server_clients}');
  $page = array();
  foreach ($result as $record) {
    $build = array();
    $rows = array();
    $rows[] = array('Community', $record->name);
    $rows[] = array('URL', $record->url);
    $rows[] = array('Description', $record->description);
    $server = url($record->url . "/xmlrpc.php");
    $rpc_result = xmlrpc(
      $server,
      array('statisticsClient.info' => array())
    );
    if ($rpc_result === FALSE) {
      $rows[] = array('', 'Error retrieving information.');
    }
    else {
      $rows[] = array('Client version', isset($rpc_result['client_version']) ? $rpc_result['client_version'] : 'N/A');
      $rows[] = array('Local time', isset($rpc_result['time']) ? $rpc_result['time'] : 'N/A');
      $rows[] = array('Drupal version', isset($rpc_result['drupal_version']) ? $rpc_result['drupal_version'] : 'drupal_version');
      $rows[] = array('Web server', isset($rpc_result['web_server']) ? $rpc_result['web_server'] : 'N/A');
      $rows[] = array('PHP version', isset($rpc_result['php']) ? $rpc_result['php'] : 'N/A');
      $rows[] = array('DB version', isset($rpc_result['db']) ? $rpc_result['db'] : 'N/A');
      $rows[] = array('Users in communinty', isset($rpc_result['n_users']) ? $rpc_result['n_users'] : 'N/A');
      $rows[] = array('Installed modules', isset($rpc_result['modules']) ? implode(', ', $rpc_result['modules']) : '');
    }
    $build['node_table'] = array(
      '#theme' => 'table',
      '#header' => array('Name', 'Value'),
      '#rows' => $rows,
      '#empty' => t('No content types available.'),
      '#prefix' => '<div><h2>' . $record->name . '</h2>',
      '#suffix' => '</div>',
    );
    $page[] = $build;
  }
  return $page;
}

function _statistics_server_best_paper() {
  $result = db_query('SELECT id, name, url, description, type FROM {statistics_server_clients}');
  $rows = array();
  foreach ($result as $record) {
    $server = url($record->url . "/xmlrpc.php");
    $row = array();
    $rpc_result = xmlrpc(
      $server,
      array('statisticsClient.query' => array('SELECT id, author, title, rating, votes FROM {statistics_client_paper_' . $record->type . '} ORDER BY rating DESC LIMIT 1'))
    );
    if ($rpc_result === FALSE) {
      $rows[] = array($record->name, 'Error retrieving information.');
    }
    else {
      foreach ($rpc_result as $line) {
        $row = array();
        $row[] = $record->name;
        $row[] = $line['author'];
        $row[] = $line['title'];
        $row[] = $line['rating'];
        $row[] = $line['votes'];
      }
      $rows[] = $row;
    }
  }
  $build['node_table'] = array(
    '#theme' => 'table',
    '#header' => array('Community', 'Author', 'Title', 'Rating', 'Votes'),
    '#rows' => $rows,
    '#empty' => t('No information available.'),
    '#prefix' => '<div><h2>Best papers</h2>',
    '#suffix' => '</div>',
  );
  return $build;
}
function _statistics_server_best_news() {
  $result = db_query('SELECT id, name, url, description, type FROM {statistics_server_clients}');
  $rows = array();
  foreach ($result as $record) {
    $server = url($record->url . "/xmlrpc.php");
    $row = array();
    $rpc_result = xmlrpc(
      $server,
      array('statisticsClient.query' => array('SELECT id, author, title, rating, votes, created FROM {statistics_client_news_' . $record->type . '} ORDER BY rating DESC LIMIT 1'))
    );
    if ($rpc_result === FALSE) {
      $rows[] = array($record->name, 'Error retrieving information.');
    }
    else {
      foreach ($rpc_result as $line) {
        $row = array();
        $row[] = $record->name;
        $row[] = $line['author'];
        $row[] = $line['title'];
        $row[] = $line['rating'];
        $row[] = $line['votes'];
        $row[] = date('Y-m-d', $line['created']);
      }
      $rows[] = $row;
    }
  }
  $build['node_table'] = array(
    '#theme' => 'table',
    '#header' => array('Community', 'Author', 'Title', 'Rating', 'Votes', 'Date'),
    '#rows' => $rows,
    '#empty' => t('No information available.'),
    '#prefix' => '<div><h2>Best rated news</h2>',
    '#suffix' => '</div>',
  );
  return $build;
}
function _statistics_server_latest_news() {
  $result = db_query('SELECT id, name, url, description, type FROM {statistics_server_clients}');
  $rows = array();
  foreach ($result as $record) {
    $server = url($record->url . "/xmlrpc.php");
    $row = array();
    $rpc_result = xmlrpc(
      $server,
      array('statisticsClient.query' => array('SELECT id, author, title, rating, votes, created FROM {statistics_client_news_' . $record->type . '} ORDER BY created DESC LIMIT 1'))
    );
    if ($rpc_result === FALSE) {
      $rows[] = array($record->name, 'Error retrieving information.');
    }
    else {
      foreach ($rpc_result as $line) {
        $row = array();
        $row[] = $record->name;
        $row[] = $line['author'];
        $row[] = $line['title'];
        $row[] = $line['rating'];
        $row[] = $line['votes'];
        $row[] = date('Y-m-d', $line['created']);
      }
      $rows[] = $row;
    }
  }
  $build['node_table'] = array(
    '#theme' => 'table',
    '#header' => array('Community', 'Author', 'Title', 'Rating', 'Votes', 'Date'),
    '#rows' => $rows,
    '#empty' => t('No information available.'),
    '#prefix' => '<div><h2>Latest news</h2>',
    '#suffix' => '</div>',
  );
  return $build;
}
function _statistics_server_userstat() {
  $result = db_query('SELECT id, name, url, description, type FROM {statistics_server_clients}');
  $rows = array();
  foreach ($result as $record) {
    $server = url($record->url . "/xmlrpc.php");
    $row = array();
    $rpc_result = xmlrpc(
      $server,
      array('statisticsClient.query' => array('SELECT COUNT(id) as count, AVG(posts) as posts, AVG(votes) as votes FROM {statistics_client_user_' . $record->type . '}'))
    );
    if ($rpc_result === FALSE) {
      $rows[] = array($record->name, 'Error retrieving information.');
    }
    else {
      foreach ($rpc_result as $line) {
        $row = array();
        $row[] = $record->name;
        $row[] = $line['count'];
        $row[] = $line['posts'];
        $row[] = $line['votes'];
      }
      $rows[] = $row;
    }
  }
  $build['node_table'] = array(
    '#theme' => 'table',
    '#header' => array('Community', 'Number of users', 'Average post count', 'Average vote count'),
    '#rows' => $rows,
    '#empty' => t('No information available.'),
    '#prefix' => '<div><h2>User statistics</h2>',
    '#suffix' => '</div>',
  );
  return $build;
}
function _statistics_server_compare_rating($a, $b) {
  return $a['3'] < $b['3'];
}
function _statistics_server_compare_count($a, $b) {
  return $a['1'] > $b['1'];
}
function _statistics_server_all_news() {
  $result = db_query('SELECT id, name, url, description, type FROM {statistics_server_clients}');
  $rows = array();
  foreach ($result as $record) {
    $server = url($record->url . "/xmlrpc.php");
    $row = array();
    $rpc_result = xmlrpc(
      $server,
      array('statisticsClient.query' => array('SELECT id, author, title, rating, votes, created FROM {statistics_client_news_' . $record->type . '} ORDER BY created DESC'))
    );
    if ($rpc_result === FALSE) {
      $rows[] = array($record->name, 'Error retrieving information.');
    }
    else {
      foreach ($rpc_result as $line) {
        $row = array();
        $row[] = $record->name;
        $row[] = $line['author'];
        $row[] = $line['title'];
        $row[] = $line['rating'];
        $row[] = $line['votes'];
        $row[] = date('Y-m-d', $line['created']);
        $rows[] = $row;
      }
    }
  }
  usort($rows, '_statistics_server_compare_rating');
  $build['node_table'] = array(
    '#theme' => 'table',
    '#header' => array('Community', 'Author', 'Title', 'Rating', 'Votes', 'Date'),
    '#rows' => $rows,
    '#empty' => t('No information available.'),
    '#prefix' => '<div><h2>Best news orderer by rating</h2>',
    '#suffix' => '</div>',
  );
  return $build;
}
function _statistics_server_community_activity() {
  $result = db_query('SELECT id, name, url, description, type FROM {statistics_server_clients}');
  $rows = array();
  foreach ($result as $record) {
    $server = url($record->url . "/xmlrpc.php");
    $row = array();
    $rpc_result = xmlrpc(
      $server,
      array('statisticsClient.query' => array('SELECT COUNT(*) as count FROM {statistics_client_paper_' . $record->type . '}'))
    );
    if ($rpc_result === FALSE) {
      $rows[] = array($record->name, 'Error retrieving information.');
    }
    else {
      foreach ($rpc_result as $line) {
        $row = array();
        $row[] = $record->name;
        $row[] = $line['count'];
        $rows[] = $row;
      }
    }
  }
  usort($rows, '_statistics_server_compare_count');
  $build['node_table'] = array(
    '#theme' => 'table',
    '#header' => array('Community', 'Number of papers'),
    '#rows' => $rows,
    '#empty' => t('No information available.'),
    '#prefix' => '<div><h2>Community activity</h2>',
    '#suffix' => '</div>',
  );
  return $build;
}
/**
 * Give more statistics, i.e. send query via RPC to client.
 *
 * Note: for this to work query has to be on client's whitelist.
 */
function statistics_server_more() {
  $page = array();
  $page[] = _statistics_server_best_paper();
  $page[] =_statistics_server_best_news();
  $page[] =_statistics_server_latest_news();
  $page[] = _statistics_server_userstat();
  $page[] = _statistics_server_all_news();
  $page[] = _statistics_server_community_activity();
  return $page;
}
